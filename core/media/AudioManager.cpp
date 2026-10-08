#include "AudioManager.h"

#include <QPermissions>
#include <QMediaDevices>
#include <QMediaFormat>

#include <QGuiApplication>

AudioManager::AudioManager(QObject *parent)
    : QObject(parent), m_devices(new QMediaDevices(this)),
    m_timer(new QTimer(this))
{
    refreshDeviceLists();
    connect(m_devices, &QMediaDevices::audioInputsChanged,
            this, &AudioManager::refreshDeviceLists);
    connect(m_devices, &QMediaDevices::audioOutputsChanged,
            this, &AudioManager::refreshDeviceLists);

    m_timer->setInterval(30);
    connect(m_timer, &QTimer::timeout, this, [this] {
        if (!m_io) return;
        char buf[4096];
        const qsizetype n = m_io->read(buf, sizeof(buf));
        if (n > 0) processSamples(buf, n);
    });
}

void AudioManager::selectInput(const QString &deviceId)
{
    m_inputDevice = findDevice(m_inputs, deviceId);
    if (m_inputDevice.isNull()) m_inputDevice = QMediaDevices::defaultAudioInput();
    emit selectedInputChanged();
    if (m_metering) { stopMeter(); startMeter(); }
    qDebug() << "Input device:" << m_inputDevice.description();
}

void AudioManager::selectOutput(const QString &deviceId)
{
    m_outputDevice = findDevice(m_outputs, deviceId);
    if (m_outputDevice.isNull()) m_outputDevice = QMediaDevices::defaultAudioOutput();
    emit selectedOutputChanged();
    qDebug() << "Output device:" << m_outputDevice.description()
             << "preferred:" << m_outputDevice.preferredFormat().sampleRate() << "Hz";
}

void AudioManager::startMeter()
{
    if(m_metering) stopMeter();

    QMicrophonePermission micPermission;
    switch (qGuiApp->checkPermission(micPermission)) {
    case Qt::PermissionStatus::Undetermined:
        qGuiApp->requestPermission(micPermission, this, [this] { startMeter(); });
        return;
    case Qt::PermissionStatus::Denied:
        qWarning() << "Microphone permission denied";
        return;
    case Qt::PermissionStatus::Granted:
        break;
    }

    if (m_inputDevice.isNull()) {
        m_inputDevice = QMediaDevices::defaultAudioInput();
        emit selectedInputChanged();
    }

    const QAudioFormat fmt = m_inputDevice.preferredFormat();
    qDebug() << "Mic start:" << m_inputDevice.description()
             << fmt.sampleRate() << "Hz," << fmt.channelCount() << "ch,"
             << "fmt:" << fmt.sampleFormat();

    m_source = new QAudioSource(m_inputDevice, fmt, this);
    m_io = m_source->start();
    if (!m_io) {
        qWarning() << "Failed to start audio input";
        m_source->deleteLater();
        m_source = nullptr;
        return;
    }
    m_timer->start();
    m_metering = true;
    emit meteringChanged();
}

void AudioManager::processSamples(const void *data, qsizetype bytes)
{
    const QAudioFormat::SampleFormat sf =
        m_source->format().sampleFormat();
    double sum = 0.0;
    qsizetype count = 0;

    if (sf == QAudioFormat::Int16) {
        const auto *s = static_cast<const qint16 *>(data);
        count = bytes / 2;
        for (qsizetype i = 0; i < count; ++i) {
            const double v = s[i] / 32768.0;
            sum += v * v;
        }
    } else if (sf == QAudioFormat::Float) {
        const auto *s = static_cast<const float *>(data);
        count = bytes / 4;
        for (qsizetype i = 0; i < count; ++i)
            sum += double(s[i]) * double(s[i]);
    } else if (sf == QAudioFormat::Int32) {
        const auto *s = static_cast<const qint32 *>(data);
        count = bytes / 4;
        for (qsizetype i = 0; i < count; ++i) {
            const double v = s[i] / 2147483648.0;
            sum += v * v;
        }
    } else if (sf == QAudioFormat::UInt8) {
        const auto *s = static_cast<const quint8 *>(data);
        count = bytes;
        for (qsizetype i = 0; i < count; ++i) {
            const double v = (s[i] - 128.0) / 128.0;
            sum += v * v;
        }
    } else {
        return;
    }

    if (count == 0) return;
    m_level = float(qBound(0.0, std::sqrt(sum / count), 1.0));
    emit inputLevelChanged(m_level);
}

void AudioManager::stopMeter()
{
    if (!m_source) return;
    m_timer->stop();
    m_source->stop();
    m_io = nullptr;
    m_source->deleteLater();
    m_source = nullptr;
    m_metering = false;
    emit meteringChanged();
}

AudioManager::~AudioManager() { stopMeter(); }

QVariantList AudioManager::audioInputs() const  { return m_inputs; }
QVariantList AudioManager::audioOutputs() const { return m_outputs; }

QString AudioManager::selectedInputId() const
{
    return QString::fromUtf8(m_inputDevice.id());
}

QString AudioManager::selectedOutputId() const
{
    return QString::fromUtf8(m_outputDevice.id());
}

bool AudioManager::metering() const   { return m_metering; }
float AudioManager::inputLevel() const { return m_level; }

void AudioManager::refreshDeviceLists()
{
    auto toMap = [](const QList<QAudioDevice> &devices) {
        QVariantList result;
        for (const QAudioDevice &d : devices) {
            result.append(QVariantMap{
                { "id",        QString::fromUtf8(d.id()) },
                { "name",      d.description() },
                { "isDefault", d.isDefault() },
                });
        }
        return result;
    };

    m_inputs  = toMap(m_devices->audioInputs());
    m_outputs = toMap(m_devices->audioOutputs());
    emit audioInputsChanged();
    emit audioOutputsChanged();
}

QAudioDevice AudioManager::findDevice(const QVariantList &list, const QString &deviceId) const
{
    Q_UNUSED(list);
    const auto search = [](const QList<QAudioDevice> &devices, const QString &id) {
        for (const QAudioDevice &d : devices)
            if (QString::fromUtf8(d.id()) == id)
                return d;
        return QAudioDevice{};
    };
    QAudioDevice d = search(m_devices->audioInputs(), deviceId);
    if (!d.isNull())
        return d;
    return search(m_devices->audioOutputs(), deviceId);
}