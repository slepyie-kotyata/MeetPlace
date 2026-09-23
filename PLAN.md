# MeetPlace — MVP «аналог Zoom»

> Десктопное приложение для видеозвонков на Qt 6 (C++ / QML) с P2P mesh до 4 участников.
> Срок MVP: 4 месяца (4 спринта по 2 недели), команда — 3 разработчика.

---

## 1. Ключевые решения

| Решение | Выбор | Обоснование |
|---|---|---|
| Фреймворк | Qt 6.7+ / C++20 / QML | Цель проекта — продемонстрировать навыки Qt (портфолио) |
| WebRTC-стек | **libdatachannel** | Лёгкая C++ библиотека, отличная документация, Qt-совместима |
| Топология | **P2P mesh** (до 4 участников) | Только сигнальный сервер + STUN/TURN, без SFU |
| Сигналинг | WebSocket, JSON-протокол | Node.js-сервер (~200 строк) |
| Чат | WebRTC DataChannel | Не нагружает сигнальный сервер, чище архитектурно |
| NAT-обход | STUN (Google) + TURN (coturn) как fallback | TURN нужен для симметричных NAT |
| Платформы MVP | Windows, macOS, Linux | Android/iOS — этап 2 (после MVP) |
| Сборка | CMake + GitHub Actions (3 ОС) | CI с первой недели |
| Роли | Dev1 — UI Lead, Dev2 — Media, Dev3 — Network/Logic | См. §3 |

### Вне scope MVP (этап 2)
- Запись звонков, реакции, breakout rooms
- Аккаунты/авторизация/контакты (MVP — комнаты по ID)
- SFU и звонки > 4 участников
- Android / iOS (отдельный спринт 1–1.5 мес: NDK-сборка libdatachannel, QML под mobile)
- Дополнительное шифрование (в P2P WebRTC DTLS/SRTP уже даёт e2e)

---

## 2. Архитектура

```
┌─ QML UI ──────────────────────────────────────┐
│  VideoGrid │ ChatPanel │ CallBar │ Settings   │
└──────┬────────────────────────────────────────┘
       │ (C++ типы, экспортированные в QML:
       │  CallController, ParticipantModel,
       │  ChatModel, SettingsManager)
┌─ C++ Core ────────────────────────────────────┐
│  CallController (state machine звонка)        │
│  ┌──────────────┬─────────────┬─────────────┐ │
│  │ MediaEngine  │ PeerManager │ SignalClient│ │
│  │ (capture,    │ (libdatach. │ (WebSocket) │ │
│  │  encode,     │  peers,     │             │ │
│  │  decode)     │  DataChannel│             │ │
│  └──────────────┴─────────────┴─────────────┘ │
└───────────────────────────────────────────────┘
        ↕ WebSocket (JSON)          ↕ SRTP/UDP
   Signaling server (Node.js)    P2P mesh peers
```

### Состояния CallController

```
Idle → Connecting → Connected → (Error → Idle)
                     ↓
                   Disconnected → Idle
```

---

## 3. Роли

| Разработчик | Зона | Ключевые Qt-компоненты |
|---|---|---|
| **Dev1 (UI Lead)** — сильнее в UI/дизайне | QML-интерфейс, темы, анимации, экраны ошибок | Qt Quick Controls, GridView/ListView, QML Singleton, Transitions, биндинги к C++ |
| **Dev2 (Media)** | Захват и рендеринг аудио/видео, screen share, mute | QMediaDevices, QCamera, QVideoSink, QVideoFrame, QThread |
| **Dev3 (Network/Logic)** | Сигналинг, WebRTC-сессии, чат-модель, переподключение | QWebSocket, QJsonDocument, QAbstractListModel, Q_PROPERTY |

**Правило «second»:** у каждого модуля есть второй владелец из другой зоны (Dev1↔Dev2, Dev2↔Dev3, Dev3↔Dev1) — страховка от болезни/выпадения участника.

---

## 4. Протокол сигналинга (контракт, фиксируется в спринте 1)

Транспорт: WebSocket, все сообщения — JSON-объекты.

### Сообщения клиент → сервер

```json
// Войти в комнату
{ "type": "join",    "roomId": "abc123", "peerId": "peer-uuid", "name": "Alice" }

// Покинуть комнату
{ "type": "leave",   "roomId": "abc123", "peerId": "peer-uuid" }

// Relay WebRTC-сигналов конкретному пиру
{ "type": "offer",   "roomId": "abc123", "from": "peer-1", "to": "peer-2", "payload": { "sdp": "..." } }
{ "type": "answer",  "roomId": "abc123", "from": "peer-1", "to": "peer-2", "payload": { "sdp": "..." } }
{ "type": "ice",     "roomId": "abc123", "from": "peer-1", "to": "peer-2", "payload": { "candidate": "...", "sdpMid": "0", "sdpMLineIndex": 0 } }
```

### Сообщения сервер → клиент

```json
// Подтверждение входа + список уже присутствующих
{ "type": "joined",  "roomId": "abc123", "peerId": "peer-uuid",
  "peers": [ { "peerId": "peer-1", "name": "Bob" } ] }

// Новый участник вошёл (всем, кроме него)
{ "type": "peer-joined", "roomId": "abc123", "peerId": "peer-3", "name": "Carol" }

// Участник вышел
{ "type": "peer-left",   "roomId": "abc123", "peerId": "peer-3" }

// Relay сигналов (тот же формат, что и client → server)

// Ошибка
{ "type": "error",   "code": "room-full", "message": "Room is full (max 4)" }
```

### Правила mesh
- Присоединившийся отправляет **offer** каждому уже присутствующему.
- ICE-кандидаты relay'ятся точечно (`to`).
- Сервер ограничивает комнату 4 пира́ми.
- Сервер не хранит историю и не содержит бизнес-логики — только relay + учёт пиров.

### Chat (через DataChannel, не через сигналинг)

```json
{ "kind": "chat", "text": "привет", "ts": 1730000000, "name": "Alice" }
```

---

## 5. Интерфейс CallController (контракт)

```cpp
class CallController : public QObject {
    Q_OBJECT
    // Q_PROPERTY: state (Idle/Connecting/Connected/Error), roomJoined, muted, cameraOn, screenSharing
public:
    Q_INVOKABLE void joinRoom(const QString &roomId, const QString &name);
    Q_INVOKABLE void leaveRoom();
    Q_INVOKABLE void toggleMute();
    Q_INVOKABLE void toggleCamera();
    Q_INVOKABLE void startScreenShare();
    Q_INVOKABLE void stopScreenShare();
    Q_INVOKABLE void sendChatMessage(const QString &text);
signals:
    void stateChanged();
    void participantJoined(const QString &peerId, const QString &name);
    void participantLeft(const QString &peerId);
    void errorOccurred(const QString &code, const QString &message);
};
```

- Модель участников: `ParticipantModel : QAbstractListModel` (peerId, name, muted, cameraOn, speaking, QVideoFrame sink)
- Модель чата: `ChatModel : QAbstractListModel` (name, text, timestamp, direction)

---

## 6. Структура репозитория

```
MeetPlace/
├── app/                    # GUI-приложение
│   ├── main.cpp
│   └── qml/
│       ├── main.qml
│       ├── screens/        # StartScreen, CallScreen, ErrorScreen
│       ├── components/     # VideoTile, CallBar, ChatPanel, ScreenPicker
│       └── theme/          # Theme.qml (singleton, dark/light)
├── core/                   # C++-ядро (без зависимостей от QML)
│   ├── call/               # CallController
│   ├── media/              # LocalCameraManager, AudioManager, ScreenCapture
│   ├── net/                # SignalClient, PeerManager, ChatChannel
│   └── models/             # ParticipantModel, ChatModel
├── signaling-server/       # Node.js (ws): комнаты + relay
├── tests/                  # юнит- и интеграционные тесты
├── .github/workflows/      # CI: build win/mac/linux + clang-format
├── CMakeLists.txt
└── PLAN.md
```

---

## 7. Спринт 1 (недели 1–2): Каркас и вертикальные срезы

**Цель:** общий CMake-проект собирается в CI на 3 ОС; каждый имеет работающий минимальный кусок.

Первые 3 дня всей команды — только изучение: signals/slots, threading model, QML↔C++ бридж.

| Кто | Задачи | DoD |
|---|---|---|
| Все | Структура репо (§6), CMake, GitHub Actions (сборка 3 ОС + clang-format), библиотека libdatachannel подключается в CMake | CI зелёный на 3 ОС |
| Dev1 | Стартовый экран (имя + ID комнаты), окно звонка (VideoGrid-заглушка, CallBar: Mute/Camera/Leave), Theme Singleton (dark/light), моковые данные | Навигация экранов работает, темы переключаются |
| Dev2 | `LocalCameraManager`: QMediaDevices → QCamera → QVideoSink → self-view в QML. `AudioManager`: список устройств, уровень микрофона в QML-индикатор. Исследовать конвертацию QVideoFrame → формат libdatachannel | Своё видео с камеры видно, индикатор аудио живой; **проверить macOS permissions (NSCameraUsageDescription)** |
| Dev3 | Сигнальный сервер (Node.js ws): комнаты, relay, лимит 4. `SignalClient` на QWebSocket + JSON-протокол §4. Каркас `CallController` (state machine) | Интеграционный тест: 2 клиента обмениваются join/offer/ice через сервер |

**Риски:** капризы камеры/микрофона и permissions на macOS — проверять сразу, не в конце.

---

## 8. Спринт 2 (недели 3–4): Первый P2P видеозвонок ⭐

**Главная веха проекта.** Чекпойнт в конце недели 4: если 1-на-1 звонок не работает — пересматриваем план (fallback: уменьшить scope до 1-на-1).

| Кто | Задачи | DoD |
|---|---|---|
| Dev2 | Интеграция libdatachannel: `PeerConnection`, audio/video track, отправка кадров камеры, приём входящих → QVideoFrame → QML. *(Самая сложная задача проекта; может звать Dev3 на помощь)* | Локальный кадр уходит в трек, входящий кадр рисуется в QML |
| Dev3 | Склейка сигналинга и libdatachannel: обмен SDP offer/answer + ICE. Логика mesh: join → offer каждому присутствующему. STUN: `stun:stun.l.google.com:19302` | 2 клиента в LAN совершают звонок |
| Dev1 | Реальный VideoGrid: `ParticipantModel` (QAbstractListModel) → GridView/Repeater, VideoOutput на каждого удалённого участника. Состояния звонка в UI (подключение/ошибка/разговор). Тест на 2 машинах в LAN, багрепорты | UI показывает реальных участников и статус звонка |

**DoD спринта:** 2 разных устройства в одной сети, звонок 1-на-1 с видео и аудио, latency < 500 мс.

---

## 9. Спринт 3 (недели 5–6): Mesh на 4 участника + чат + mute + screen share

| Кто | Задачи | DoD |
|---|---|---|
| Dev2 | Mute/unmute mic и камеры на лету (без разрыва соединения). Уровень микрофона → индикатор «кто говорит» (сигнал в QML). Screen share: захват через QScreen/QCapturableWindow, переключение источника трека | Mute/камера/шеринг работают у локального пира |
| Dev3 | `ChatModel` (QAbstractListModel) + сообщения через **DataChannel**. Mesh-тест на 3–4 участниках: добавление/выход посреди звонка. Пинги/статусы соединений | Чат 1-на-1 и mesh на 4 стабилен |
| Dev1 | Панель чата (ListView, бабблы, автоскролл, бейдж новых сообщений). UI выбора экрана/окна. Список участников с индикаторами mute/камеры. Индикатор говорящего | Все элементы UI подключены к реальным данным |

**DoD спринта:** 4 участника в mesh, чат работает, mute/камера/экран переключаются у всех.

---

## 10. Спринт 4 (недели 7–8): Стабилизация и релиз

| Кто | Задачи | DoD |
|---|---|---|
| Все | Багфикс-марафон: документ «Known Issues» + приоритизация. Тесты сценариев: Wi-Fi + Ethernet, выход посреди звонка, повторное подключение, 4 участника на слабых машинах | Нет blocker/critical багов |
| Dev2 | Профилирование: CPU при 4 потоках (проверить конвертацию пиксельных форматов — обычно 90% проблем). Адаптация разрешения/битрейта при слабой сети | Приемлемая нагрузка CPU на 4 потоках |
| Dev3 | Reconnect при обрыве WebSocket + восстановление peer-соединений. Очистка ресурсов (память, сокеты) при выходе. TURN-сервер (coturn) как fallback для симметричных NAT | Звонок переживает кратковременный обрыв сети |
| Dev1 | Полировка UI: анимации (Transitions), empty/error-состояния, тултипы, иконки. Экран ошибки камеры/микрофона. Упаковка: windeployqt / macdeployqt / Linux AppImage | Установщики на 3 ОС собираются в CI |
| Все | README: архитектурная диаграмма, скриншоты, демо-GIF. **Демо-видео 2–3 мин** | README + видео опубликованы |

**DoD MVP:** стабильный звонок на 4 участника (видео, аудио, чат, шеринг экрана) на 3 ОС, установщики и демо-видео готовы.

---

## 11. Этап 2 (после MVP, месяцы 5+)

1. Android (NDK-сборка libdatachannel, Qt для Android, адаптация QML) — ~1–1.5 мес
2. iOS (только по необходимости; тестирование ограничено эмулятором)
3. SFU (mediasoup/livekit) для > 4 участников
4. Запись звонков, реакции, аккаунты

---

## 12. Риски и митигация

| Риск | Митигация |
|---|---|
| Спринт 2 сорвётся (WebRTC сложнее, чем казалось) | Честный чекпойнт в конце недели 4; fallback — сузить до 1-на-1 |
| Участник выпадает на неделю | Контракты §4–5 с первой недели; правило «second» (§3) |
| Permissions камеры/микрофона на macOS | Dev2 проверяет уже в спринте 1 |
| Интеграция развалится в конце | Контракты §4–5; интеграционные сборки в CI каждую неделю |
| 10–20 ч/нед на человека мало | Демо по пятницам (хотя бы скриншот),weekly синк 30–60 мин, задачи в GitHub Projects, PR с код-ревью |

---

## 13. Процесс работы

- **Weekly синк** 30–60 мин: статус, блокеры, планирование недели
- **Демо по пятницам** в общем чате (скриншот/запись)
- **GitHub Projects:** все задачи в трекере; ветки + PR с код-ревью друг у друга
- **CI:** каждая неделя заканчивается собираемым проектом на всех 3 ОС
