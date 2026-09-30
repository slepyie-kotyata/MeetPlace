#include "LocalCameraManager.h"

#include <QVideoFrame>
#include <QDebug>
#include <QGuiApplication>
#include <QPermissions>

LocalCameraManager::LocalCameraManager(QObject *parent):
    QObject(parent), m_devices(new QMediaDevices(this)), m_session(new QMediaCaptureSession(this))
{
    refreshCameraList();

    connect(m_devices, &QMediaDevices::videoInputsChanged, this, &LocalCameraManager::refreshCameraList);
}

LocalCameraManager::~LocalCameraManager()
{
    stop();
}

QVariantList LocalCameraManager::cameras() const { return m_cameras; }

bool LocalCameraManager::active() const { return m_active; }

QVideoSink *LocalCameraManager::videoSink() const
{
    return m_session->videoSink();
}


void LocalCameraManager::setVideoSink(QVideoSink *sink)
{
    QVideoSink *old = m_session->videoSink();
    if (old == sink)
        return;
    if (old)
        disconnect(old, nullptr, this, nullptr);

    m_session->setVideoSink(sink);

    if (sink) {
        connect(sink, &QVideoSink::videoFrameChanged,
                this, [this](const QVideoFrame &frame) {
                    if (frame.isValid()) {
                        qDebug() << "frame:" << frame.width() << "x" << frame.height()
                        << "fmt:" << frame.pixelFormat();
                        emit frameArrived(frame.width(), frame.height(),
                                          int(frame.pixelFormat()));
                    }
                });
    }
    emit videoSinkChanged();
}

void LocalCameraManager::start(const QString &deviceId)
{
    QCameraPermission cameraPermission;

    switch (qGuiApp->checkPermission(cameraPermission)) {
    case Qt::PermissionStatus::Undetermined:
        qGuiApp->requestPermission(cameraPermission, this, [this, deviceId] {
            start(deviceId);
        });
        return;
    case Qt::PermissionStatus::Denied:
        qWarning() << "Camera permission denied — check System Settings → Privacy → Camera";
        return;
    case Qt::PermissionStatus::Granted:
        break;
    }

    const QCameraDevice device = findDevice(deviceId);
    if (device.isNull()) {
        qWarning() << "Camera not found:" << deviceId;
        return;
    }

    stop();

    m_camera = new QCamera(device, this);
    m_session->setCamera(m_camera);
    m_camera->start();

    m_active = true;
    emit activeChanged();
    qDebug() << "Camera started:" << device.description()
             << "default format:" << m_camera->cameraFormat().pixelFormat();
}

void LocalCameraManager::stop()
{
    if (!m_camera)
        return;
    m_camera->stop();
    m_session->setCamera(nullptr);
    m_camera->deleteLater();
    m_camera = nullptr;

    m_active = false;
    emit activeChanged();
}

void LocalCameraManager::refreshCameraList()
{
    m_cameras.clear();
    const auto inputs = m_devices->videoInputs();
    m_cameras.reserve(inputs.size());
    for (const QCameraDevice &d : inputs) {
        m_cameras.append(QVariantMap{
            { "id",   d.id() },
            { "name", d.description() },
            });
    }
    emit camerasChanged();
}

QCameraDevice LocalCameraManager::findDevice(const QString &deviceId) const
{
    if (deviceId.isEmpty())
        return QMediaDevices::defaultVideoInput();

    const auto inputs = m_devices->videoInputs();
    for (const QCameraDevice &d : inputs) {
        if (d.id() == deviceId)
            return d;
    }
    return {};
}
