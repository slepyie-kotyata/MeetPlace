#include "PeerModel.h"
#include <qtmetamacros.h>

PeerModel::PeerModel(QObject *parent)
    : QAbstractListModel(parent) {};

int PeerModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return peers.size();
};

QVariant PeerModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= peers.size()) {
        return QVariant();
    }
    const Peer &peer = peers.at(index.row());

    switch (role) {
        case PeerIdRole:
            return peer.peerId;
        case NameRole:
            return peer.name;
        case MicRole:
            return peer.isMicOn;
        case CameraRole:
            return peer.isCameraOn;
        default:
            return QVariant();
    }
};

QHash<int, QByteArray> PeerModel::roleNames() const {
    QHash<int, QByteArray> roles;

    roles[PeerIdRole] = "peerId";
    roles[NameRole] = "name";
    roles[MicRole] = "isMicOn";
    roles[CameraRole] = "isCameraOn";
    return roles;
};

int PeerModel::findPeer(const QString &peerId) const {
    for (int i = 0; i < peers.size(); ++i) {
        if (peers[i].peerId == peerId) {
            return i;
        }
    }
    return -1;
};

void PeerModel::addPeer(const QString &peerId, const QString &name) {
    if (findPeer(peerId) != -1) {
        return;
    }
    int row = peers.size();

    beginInsertRows(QModelIndex(), row, row);
    Peer peer;
    peer.peerId = peerId;
    peer.name = name;
    peers.append(peer);
    endInsertRows();
};

void PeerModel::removePeer(const QString &peerId) {
    int row = findPeer(peerId);
    if (row == -1) {
        return;
    }

    beginRemoveRows(QModelIndex(), row, row);
    peers.removeAt(row);
    endRemoveRows();
};

void PeerModel::setPeerMic(const QString &peerId, bool micOn) {
    int row = findPeer(peerId);

    if (row == -1) {
        return;
    } else if (peers[row].isMicOn == micOn) {
        return;
    } else {
        peers[row].isMicOn = micOn;
        notifyPeerChanged(row, MicRole);
    }
}

void PeerModel::setPeerCamera(const QString &peerId, bool cameraOn) {
    int row = findPeer(peerId);

    if (row == -1) {
        return;
    } else if (peers[row].isCameraOn == cameraOn) {
        return;
    } else {
        peers[row].isCameraOn = cameraOn;
        notifyPeerChanged(row, CameraRole);
    }
};

void PeerModel::notifyPeerChanged(int row, int role) {
    QModelIndex idx = index(row);
    emit dataChanged(idx, idx, {role});
};