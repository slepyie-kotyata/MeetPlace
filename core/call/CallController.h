#ifndef CALLCONTROLLER_H
#define CALLCONTROLLER_H

#include <QObject>
#include <QString>
#include <qtmetamacros.h>
#include "net/SignalClient.h"

// Q_PROPERTY: state (Idle/Connecting/Connected/Error), roomJoined(я пока хз зачем оно), muted, cameraOn, screenSharing

class CallController : public QObject {
    Q_OBJECT
    enum State{Idle, Connecting, Connected, Error};
    Q_ENUM(State)
    Q_PROPERTY(State state READ getState WRITE setState NOTIFY stateChanged)
signals:
    void stateChanged();
    void participantJoined(const QString &peerId, const QString &name);
    void participantLeft(const QString &peerId);
    void errorOccurred(const QString &code, const QString &message);
public:
    CallController(QObject *parent = nullptr);
    State getState() const;
    Q_INVOKABLE void joinCall(const QString &roomId, const QString &peerId);
    Q_INVOKABLE void leaveCall();
private slots:
    void handleMessage(const QJsonObject &message);
    void handleError(const QString &errorStr);
private:
    State state;
    SignalClient *sygClient;
    void setState(State state);
};
#endif