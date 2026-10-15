#ifndef CALLCONTROLLER_H
#define CALLCONTROLLER_H

#include <QObject>
#include <QString>
#include <qtmetamacros.h>

// Q_PROPERTY: state (Idle/Connecting/Connected/Error), roomJoined(я пока хз зачем оно), muted, cameraOn, screenSharing

class CallController : public QObject {
    Q_OBJECT
    enum State{Idle, Connecting, Connected, Error};
    
    Q_ENUM(State)
    Q_PROPERTY(State state READ getState WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(bool muted READ isMuted WRITE setMuted NOTIFY mutedSet)
    Q_PROPERTY(bool camera READ getCamera WRITE setCamera NOTIFY cameraChanged)
    Q_PROPERTY(bool screenSharing READ getScreenSharing WRITE setScreenSharing NOTIFY screenSharingChanged)
public:
    State getState() const;
    bool isMuted() const;
    bool getCamera() const;
    bool getScreenSharing() const;
    
    void setState(State state);
    void setMuted(bool muted);
    void setCamera(bool cameraState);
    void setScreenSharing(bool screenSharing);
    
    Q_INVOKABLE void joinRoom(const QString &roomId, const QString &name);
    Q_INVOKABLE void leaveRoom();
    Q_INVOKABLE void toggleMute();
    Q_INVOKABLE void toggleCamera();
    Q_INVOKABLE void startScreenShare();
    Q_INVOKABLE void stopScreenShare();
    Q_INVOKABLE void sendChatMessage(const QString &text);
signals:
    void stateChanged();
    void mutedSet();
    void cameraChanged();
    
    void participantJoined(const QString &peerId, const QString &name);
    void participantLeft(const QString &peerId);
    void errorOccurred(const QString &code, const QString &message);
private:
    State p_state;
    bool p_muted;
    bool p_camera;
    bool p_screen_sharing;
public:
    CallController(QObject *parent = nullptr);
};
#endif