#include <QObject>
#include <QString>

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

  public:
    CallController(const CallController &) = default;
    CallController(CallController &&) = default;
    CallController &operator=(const CallController &) = default;
    CallController &operator=(CallController &&) = default;
};