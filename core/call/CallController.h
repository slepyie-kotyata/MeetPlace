#ifndef CALLCONTROLLER_H
#define CALLCONTROLLER_H

#include <QObject>
#include <QString>
#include <qobject.h>
#include <qtmetamacros.h>
#include "net/SignalClient.h"

// Q_PROPERTY: state (Idle/Connecting/Connected/Error), roomJoined(я пока хз зачем оно), muted, cameraOn, screenSharing
// возможно вынести типы ошибок в отдельную структуру

class CallController : public QObject {
    Q_OBJECT
    enum ConnState{Idle, Connecting, Connected, Error};
    Q_ENUM(ConnState)
    Q_PROPERTY(ConnState connState READ getState NOTIFY stateChanged)
    Q_PROPERTY(bool micEnabled READ isMicEnabled NOTIFY micEnabledChanged)
    Q_PROPERTY(bool cameraEnabled READ isCameraEnabled NOTIFY cameraEnabledChanged)
signals:
    // для аудио и видео движка
    void micToggleRequested(bool enable);
    void cameraToggleRequested(bool enable);
    // для QML (насчет нас)
    void stateChanged();
    void micEnabledChanged(bool enabled);
    void cameraEnabledChanged(bool enabled);
    void errorOccurred(const QString &code, const QString &message);
    // возможно перенести
    void peerJoined(const QString &peerId, const QString &name);
    void peerLeft(const QString &peerId);
public:
    CallController(QObject *parent = nullptr);
    ConnState getState() const;
    bool isMicEnabled() const;
    bool isCameraEnabled() const;
    Q_INVOKABLE void toggleMic();
    Q_INVOKABLE void toggleCamera();
    Q_INVOKABLE void joinCall(const QString &roomId, const QString &peerId);
    Q_INVOKABLE void leaveCall();
public slots:
    void micToggleConfirmed(bool enable);
    void cameraToggleConfirmed(bool enable);
private slots:
    void handleMessage(const QJsonObject &message);
    void handleError(const QString &errorStr);
private:
    QString id;
    ConnState connState;
    bool micEnabled;
    bool cameraEnabled;
    SignalClient *sigClient;
    void setState(ConnState state);
};
#endif