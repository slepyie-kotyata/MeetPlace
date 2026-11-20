#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QObject>
#include <QVariantList>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QAudioSource>
#include <QTimer>

#include <functional>

class AudioManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList audioInputs READ audioInputs NOTIFY audioInputsChanged)
    Q_PROPERTY(QVariantList audioOutputs READ audioOutputs NOTIFY audioOutputsChanged)
    Q_PROPERTY(QString selectedInputId READ selectedInputId WRITE selectInput NOTIFY selectedInputChanged)
    Q_PROPERTY(QString selectedOutputId READ selectedOutputId WRITE selectOutput NOTIFY selectedOutputChanged)
    Q_PROPERTY(bool metering READ metering NOTIFY meteringChanged)
    Q_PROPERTY(float inputLevel READ inputLevel NOTIFY inputLevelChanged)

public:
    explicit AudioManager(QObject *parent = nullptr);
    ~AudioManager() override;

    QVariantList audioInputs() const;
    QVariantList audioOutputs() const;
    QString selectedInputId() const;
    QString selectedOutputId() const;
    bool metering() const;
    float inputLevel() const;

    Q_INVOKABLE void startMeter();
    Q_INVOKABLE void stopMeter();

public slots:
    void selectInput(const QString &deviceId);
    void selectOutput(const QString &deviceId);
    // хэндшейк с CallController:
    // CallController::micToggleRequested -> сюда,
    // AudioManager::micStateChanged -> CallController::micToggleConfirmed
    void onMicToggleRequested(bool enable);

signals:
    void audioInputsChanged();
    void audioOutputsChanged();
    void selectedInputChanged();
    void selectedOutputChanged();
    void meteringChanged();
    void inputLevelChanged(float level);
    void micStateChanged(bool enabled);
    void mediaErrorOccurred(const QString &code, const QString &message);

private slots:
    void handleInputsChanged();

private:
    Qt::PermissionStatus ensureMicPermission(const std::function<void()> &onGrantedAsync);
    void resumeMicEnable();
    bool openInput();
    void closeInput();
    void refreshDeviceLists();
    QAudioDevice findDevice(const QVariantList &list, const QString &deviceId) const;
    void processSamples(const void *data, qsizetype bytes);

    QMediaDevices *m_devices = nullptr;
    QAudioSource *m_source = nullptr;
    QIODevice *m_io = nullptr;
    QTimer *m_timer = nullptr;
    QAudioDevice m_inputDevice;
    QAudioDevice m_outputDevice;
    QVariantList m_inputs;
    QVariantList m_outputs;
    float m_level = 0.0f;
    bool m_metering = false;
    bool m_awaitingMicPermission = false; // toggle(true) ждёт ответа на requestPermission
};

#endif // AUDIOMANAGER_H
