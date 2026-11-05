#ifndef PEERMODEL_H
#define PEERMODEL_H
#include <QAbstractListModel>
#include <qobject.h>
#include <qtmetamacros.h>

struct Peer {
    QString peerId;
    QString name;
    bool isMicOn = false;
    bool isCameraOn = false;
    // bool isSpeaking = false;
    // QVideoSink* videoSink = nullptr;
};

class PeerModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum PeerRoles {
        PeerIdRole = Qt::UserRole + 1,
        NameRole,
        MicRole,
        CameraRole,
        SpeakingRole
    };
    Q_ENUM(PeerRoles)
    PeerModel(QObject *parent = nullptr);
    
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void addPeer(const QString &peerId, const QString &name);
    void removePeer(const QString &peerId);
    void setPeerMic(const QString &peerId, bool micOn);
    void setPeerCamera(const QString &peerId, bool cameraOn);
signals:
    
    
private:
    QList<Peer> peers;
    int findPeer(const QString &peerId) const;
};
#endif