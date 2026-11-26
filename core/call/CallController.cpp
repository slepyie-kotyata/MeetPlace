#include "CallController.h"
#include "models/PeerModel.h"
#include "net/SignalClient.h"
#include <qlogging.h>
#include <qobject.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrlQuery>
#include <QJsonArray>

// ОСТАВЬ НАДЕЖДУ ВСЯК СЮДА ВХОДЯЩИЙ

//TODO: связать с PeerModel

namespace {
    constexpr const char* WS_URL = "wss://meetplace.enjine.ru/ws";
}

enum class MessageType {
    Unknown = 0,
    Joined,
    PeerJoined,
    PeerLeft,
    StateChanged,
    Offer,
    Ice,
    Answer,
    Error
};

static const QHash<QString, MessageType> typeMap = {
    {"joined", MessageType::Joined},
    {"peer-joined", MessageType::PeerJoined},
    {"peer-left", MessageType::PeerLeft},
    {"state-changed", MessageType::StateChanged},
    {"offer", MessageType::Offer},
    {"answer", MessageType::Answer},
    {"ice", MessageType::Ice},
    {"error", MessageType::Error},
};

CallController::CallController(QObject *parent) 
    : QObject(parent), connState(Idle), micEnabled(false), cameraEnabled(false)
{
    peerModel = new PeerModel(this);
    sigClient = new SignalClient(this);
    connect(sigClient, &SignalClient::socketDisconnected, this, [this]() {
        setState(Idle);
    });
    connect(sigClient, &SignalClient::socketError, this, &CallController::handleError);
    connect(sigClient, &SignalClient::messageReceived, this, &CallController::handleMessage);
};

void CallController::handleMessage(const QJsonObject &message) {
    QString typeStr = message.value("type").toString(); 
    QString id = message.value("peerId").toString(); 
    QString name = message.value("name").toString();
    QJsonDocument doc(message);
    QString jsonString = doc.toJson(QJsonDocument::Indented);
    
    MessageType type = typeMap.value(typeStr, MessageType::Unknown);
    switch(type){
        // сейчас при сообщении joined не передаются статус камеры и микрофона, возможны проблемы
        // TODO: перенести код из case в методы

        case MessageType::Joined: {
            setState(Connected);
            QJsonArray peers = message.value("peers").toArray();
            if (!peers.isEmpty()) {
                for (const QJsonValue &value : peers) {
                    if (!value.isObject()) {
                       continue; 
                    }
                    QJsonObject peer = value.toObject();
                    QString peerId = peer.value("peerId").toString();
                    QString peerName = peer.value("name").toString();
                    peerModel->addPeer(peerId, peerName);
                }
            }
            break;
        }
        case MessageType::PeerJoined:
            peerModel->addPeer(id, name);
            emit peerJoined(id, name);
            break;
        case MessageType::PeerLeft:
            peerModel->removePeer(id);
            emit peerLeft(id);
            break;
        case MessageType::StateChanged: {
            QString media = message.value("media").toString();
            bool isEnabled = message.value("enabled").toBool();
            
            if (media == "video") {
                peerModel->setPeerCamera(id, isEnabled);
            }
            if (media == "audio") {
                peerModel->setPeerMic(id, isEnabled);
            }
            break;
        }
        case MessageType::Offer:
            break;
        case MessageType::Answer:
            break;
        case MessageType::Ice:
            break;
        case MessageType::Error: {
            setState(Error);
            QString code = message.value("code").toString();
            QString msg = message.value("message").toString();
            emit errorOccurred(code, msg);
            break;
        }
        case MessageType::Unknown:
        default:
            qDebug() << "unknown type: " << typeStr;
            break;
    }
    qDebug() << "message: " << jsonString;
};

void CallController::handleError(const QString &errorStr) {
    emit errorOccurred("socket_error", errorStr);
    setState(Error);
};

void CallController::joinCall(const QString &roomId, const QString &peerId) {
    setState(Connecting);
    id = peerId;
    
    QUrl url(WS_URL);
    QUrlQuery query;
    query.addQueryItem("room_id", roomId);
    query.addQueryItem("peer_id", id);
    url.setQuery(query);
    
    sigClient->connectToServer(url);
};

void CallController::leaveCall() {
    sigClient->disconnectFromServer();
    setState(Idle);
};

CallController::ConnState CallController::getState() const {
    return connState;
};

void CallController::setState(ConnState state) {
    if (this->connState != state) {
        this->connState = state;
        emit stateChanged();
    }
};

bool CallController::isMicEnabled() const { return micEnabled; };
bool CallController::isCameraEnabled() const { return cameraEnabled; };

void CallController::toggleMic() {
    emit micToggleRequested(!micEnabled);
};

void CallController::toggleCamera() {
    emit cameraToggleRequested(!cameraEnabled);
};

void CallController::micToggleConfirmed(bool enable) {
    micEnabled = enable;
    emit micEnabledChanged(micEnabled);

    QJsonObject msg;
    msg["type"] = typeMap.key(MessageType::StateChanged);
    msg["peerId"] = id;
    msg["media"] = "audio";
    msg["enabled"] = micEnabled;
    sigClient->sendJson(msg);
};

void CallController::cameraToggleConfirmed(bool enable) {
    cameraEnabled = enable;
    emit cameraEnabledChanged(cameraEnabled);

    QJsonObject msg;
    msg["type"] = typeMap.key(MessageType::StateChanged);
    msg["peerId"] = id;
    msg["media"] = "video";
    msg["enabled"] = cameraEnabled;
    sigClient->sendJson(msg);
};

void CallController::mediaError(const QString code, const QString msg) {
    emit errorOccurred(code, msg);
};