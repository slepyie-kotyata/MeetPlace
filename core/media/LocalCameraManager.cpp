#include "LocalCameraManager.h"

#include <QVideoFrame>
#include <QDebug>
#include <QGuiApplication>
#include <QPermissions>

#include <algorithm>

LocalCameraManager::LocalCameraManager(QObject *parent):
    QObject(parent), m_devices(new QMediaDevices(this)), m_session(new QMediaCaptureSession(this))
{
    refreshCameraList();

    connect(m_devices, &QMediaDevices::videoInputsChanged, this, &LocalCameraManager::handleVideoInputsChanged);
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
        emit mediaErrorOccurred(QStringLiteral("permission-denied"),
            QStringLiteral("Нет доступа к камере (System Settings → Privacy → Camera)"));
        return;
    case Qt::PermissionStatus::Granted:
        break;
    }

    openCamera(deviceId);
}

bool LocalCameraManager::openCamera(const QString &deviceId)
{
    const QCameraDevice device = findDevice(deviceId);
    if (device.isNull()) {
        qWarning() << "Camera not found:" << deviceId;
        emit mediaErrorOccurred(QStringLiteral("open-failed"),
            QStringLiteral("Камера не найдена: %1").arg(deviceId));
        return false;
    }

    stop();

    m_camera = new QCamera(device, this);
    m_session->setCamera(m_camera);
    connect(m_camera, &QCamera::errorOccurred, this,
            [this](QCamera::Error error, const QString &errorString) {
        Q_UNUSED(error);
        emit mediaErrorOccurred(QStringLiteral("open-failed"), errorString);
    });
    m_camera->start();

    m_active = true;
    m_lastDeviceId = QString::fromUtf8(device.id());
    emit activeChanged();
    qDebug() << "Camera started:" << device.description()
             << "default format:" << m_camera->cameraFormat().pixelFormat();
    return true;
}

void LocalCameraManager::onCameraToggleRequested(bool enable)
{
    if (enable) {
        if (m_camera) {
            emit cameraStateChanged(true);
            return;
        }

        QCameraPermission cameraPermission;
        switch (qGuiApp->checkPermission(cameraPermission)) {
        case Qt::PermissionStatus::Undetermined:
            qGuiApp->requestPermission(cameraPermission, this, [this] {
                onCameraToggleRequested(true);
            });
            return;
        case Qt::PermissionStatus::Denied:
            qWarning() << "Camera permission denied — check System Settings → Privacy → Camera";
            emit mediaErrorOccurred(QStringLiteral("permission-denied"),
                QStringLiteral("Нет доступа к камере (System Settings → Privacy → Camera)"));
            return; // confirm не шлём
        case Qt::PermissionStatus::Granted:
            break;
        }

        if (openCamera(m_lastDeviceId)) emit cameraStateChanged(true);
        return;
    }

    if (m_camera) {
        stop();
        emit cameraStateChanged(false);
    }
}

void LocalCameraManager::handleVideoInputsChanged()
{
    refreshCameraList();

    if (!m_camera) return;
    const QString activeId = QString::fromUtf8(m_camera->cameraDevice().id());
    const auto inputs = m_devices->videoInputs();
    const bool lost = std::none_of(inputs.cbegin(), inputs.cend(),
                                   [&activeId](const QCameraDevice &d) {
                                       return QString::fromUtf8(d.id()) == activeId;
                                   });
    if (lost) {
        qWarning() << "Camera lost:" << activeId;
        stop();
        emit cameraStateChanged(false);
        emit mediaErrorOccurred(QStringLiteral("device-lost"),
            QStringLiteral("Камера отключена: %1").arg(activeId));
    }
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
