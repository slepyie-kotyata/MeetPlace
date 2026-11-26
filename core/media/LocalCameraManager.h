#ifndef LOCALCAMERAMANAGER_H
#define LOCALCAMERAMANAGER_H

#include <QObject>
#include <QVariantList>
#include <QVideoSink>
#include <QCameraDevice>
#include <QMediaDevices>
#include <QCamera>
#include <QMediaCaptureSession>

class LocalCameraManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList cameras READ cameras NOTIFY camerasChanged)
    Q_PROPERTY(bool active READ active NOTIFY activeChanged)
    Q_PROPERTY(QVideoSink *videoSink READ videoSink WRITE setVideoSink NOTIFY videoSinkChanged)

public:
    explicit LocalCameraManager(QObject *parent = nullptr);
    ~LocalCameraManager() override;

    QVariantList cameras() const;
    bool active() const;
    QVideoSink *videoSink() const;

    Q_INVOKABLE void start(const QString &deviceId = {});
    Q_INVOKABLE void stop();

public slots:
    void setVideoSink(QVideoSink *sink);
    // хэндшейк с CallController:
    // CallController::cameraToggleRequested -> сюда,
    // LocalCameraManager::cameraStateChanged -> CallController::cameraToggleConfirmed
    void onCameraToggleRequested(bool enable);

signals:
    void camerasChanged();
    void activeChanged();
    void videoSinkChanged();
    void frameArrived(int width, int height, int pixelFormat);
    void cameraStateChanged(bool enabled);
    void mediaErrorOccurred(const QString &code, const QString &message);

private slots:
    void handleVideoInputsChanged();

private:
    void refreshCameraList();
    QCameraDevice findDevice(const QString &deviceId) const;
    bool openCamera(const QString &deviceId);

    QMediaDevices *m_devices = nullptr;
    QCamera *m_camera = nullptr;
    QMediaCaptureSession *m_session = nullptr;
    QVariantList m_cameras;
    bool m_active = false;
    QString m_lastDeviceId; // последняя успешно открытая камера (для toggle)
};

#endif // LOCALCAMERAMANAGER_H
