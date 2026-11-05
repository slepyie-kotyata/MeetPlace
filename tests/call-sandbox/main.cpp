#include <QGuiApplication>
#include <QDebug>
#include <QTimer>
#include "call/CallController.h"
#include "media/AudioManager.h"
#include "media/LocalCameraManager.h"

namespace {
// CallController::ConnState приватный enum: Idle=0, Connecting=1, Connected=2
constexpr int StateConnected = 2;
}

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    CallController controller;
    AudioManager audio;
    LocalCameraManager camera;

    // --- логи CallController ---
    QObject::connect(&controller, &CallController::stateChanged, [&controller]() {
        qDebug() << "[STATE]: State changed to:" << controller.getState();
    });
    QObject::connect(&controller, &CallController::peerJoined, [](const QString &peerId, const QString &name) {
        qDebug() << "[EVENT]: Peer joined" << name << "(ID:" << peerId << ")";
    });
    QObject::connect(&controller, &CallController::peerLeft, [](const QString &peerId) {
        qDebug() << "[EVENT]: Peer left ID:" << peerId;
    });
    QObject::connect(&controller, &CallController::errorOccurred, [](const QString &code, const QString &msg) {
        qDebug() << "[ERROR]:" << code << "-" << msg;
    });
    QObject::connect(&controller, &CallController::micEnabledChanged, [](bool on) {
        qDebug() << "[CTRL]: micEnabled =" << on;
    });
    QObject::connect(&controller, &CallController::cameraEnabledChanged, [](bool on) {
        qDebug() << "[CTRL]: cameraEnabled =" << on;
    });

    // --- логи медиа-менеджеров ---
    QObject::connect(&audio, &AudioManager::micStateChanged, [](bool on) {
        qDebug() << "[AUDIO]: micStateChanged =" << on;
    });
    QObject::connect(&audio, &AudioManager::mediaErrorOccurred, [](const QString &code, const QString &msg) {
        qDebug() << "[AUDIO][ERROR]:" << code << "-" << msg;
    });
    QObject::connect(&camera, &LocalCameraManager::cameraStateChanged, [](bool on) {
        qDebug() << "[CAM]: cameraStateChanged =" << on;
    });
    QObject::connect(&camera, &LocalCameraManager::mediaErrorOccurred, [](const QString &code, const QString &msg) {
        qDebug() << "[CAM][ERROR]:" << code << "-" << msg;
    });

    // --- wiring: CallController <-> AudioManager / LocalCameraManager ---
    QObject::connect(&controller, &CallController::micToggleRequested,
                     &audio, &AudioManager::onMicToggleRequested);
    QObject::connect(&audio, &AudioManager::micStateChanged,
                     &controller, &CallController::micToggleConfirmed);
    QObject::connect(&controller, &CallController::cameraToggleRequested,
                     &camera, &LocalCameraManager::onCameraToggleRequested);
    QObject::connect(&camera, &LocalCameraManager::cameraStateChanged,
                     &controller, &CallController::cameraToggleConfirmed);
    // TODO(dev3): раскомментировать после добавления CallController::mediaError(code, msg)
    // QObject::connect(&audio, &AudioManager::mediaErrorOccurred,
    //                  &controller, &CallController::mediaError);
    // QObject::connect(&camera, &LocalCameraManager::mediaErrorOccurred,
    //                  &controller, &CallController::mediaError);

    // --- сценарий ---
    QTimer::singleShot(100, [&controller]() {
        qDebug() << "Отправляем запрос на вход в комнату 'test-room' под id 'user1'...";
        controller.joinCall("test-room", "user1");
    });

    QObject::connect(&controller, &CallController::stateChanged, [&controller, &app]() {
        if (static_cast<int>(controller.getState()) != StateConnected) return;
        qDebug() << ">>> CONNECTED — запускаем сценарий toggle:";
        QTimer::singleShot(1000, [&controller] {
            qDebug() << ">>> toggleMic() -> ON";
            controller.toggleMic();
        });
        QTimer::singleShot(4000, [&controller] {
            qDebug() << ">>> toggleMic() -> OFF";
            controller.toggleMic();
        });
        QTimer::singleShot(7000, [&controller] {
            qDebug() << ">>> toggleCamera() -> ON";
            controller.toggleCamera();
        });
        QTimer::singleShot(10000, [&controller] {
            qDebug() << ">>> toggleCamera() -> OFF";
            controller.toggleCamera();
        });
        QTimer::singleShot(14000, [&controller, &app] {
            qDebug() << ">>> Сценарий завершён. Выходим...";
            controller.leaveCall();
            app.quit();
        });
    });

    // страховка: если звонок не удался — всё равно выходим
    QTimer::singleShot(30000, [&app] {
        qWarning() << ">>> Таймаут сценария — выход";
        app.quit();
    });

    return app.exec();
};
