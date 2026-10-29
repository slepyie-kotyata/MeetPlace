#include "CallController.h"
#include "net/SignalClient.h"
#include <qlogging.h>
#include <qobject.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrlQuery>

// ОСТАВЬ НАДЕЖДУ ВСЯК СЮДА ВХОДЯЩИЙ

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
    sigClient = new SignalClient(this);
    connect(sigClient, &SignalClient::socketDisconnected, this, [this]() {
        setState(Idle);
    });
    connect(sigClient, &SignalClient::socketError, this, &CallController::handleError);
    connect(sigClient, &SignalClient::messageReceived, this, &CallController::handleMessage);
};

void CallController::handleMessage(const QJsonObject &message) {
    QString typeStr = message.value("type").toString(); 
    QString peerId = message.value("peerId").toString(); 
    QString name = message.value("name").toString();
    QJsonDocument doc(message);
    QString jsonString = doc.toJson(QJsonDocument::Indented);
    
    MessageType type = typeMap.value(typeStr, MessageType::Unknown);
    switch(type){
        case MessageType::Joined:
            setState(Connected);
            break;
        case MessageType::PeerJoined:
            emit peerJoined(peerId, name);
            break;
        case MessageType::PeerLeft:
            emit peerLeft(peerId);
            break;
        case MessageType::StateChanged:
            break;
        case MessageType::Offer:
            break;
        case MessageType::Answer:
            break;
        case MessageType::Ice:
            break;
        case MessageType::Error: {
            QString code = message.value("code").toString();
            QString msg = message.value("message").toString();
            emit errorOccurred(code, msg);
            break;
        }
        case MessageType::Unknown:
        default:
            qDebug() << "uknown type: " << typeStr;
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
    msg["type"] = "state-changed";
    msg["peerId"] = id;
    msg["media"] = "audio";
    msg["enabled"] = micEnabled;
    sigClient->sendJson(msg);
};

void CallController::cameraToggleConfirmed(bool enable) {
    cameraEnabled = enable;
    emit cameraEnabledChanged(cameraEnabled);

    QJsonObject msg;
    msg["type"] = "state-changed";
    msg["peerId"] = id;
    msg["media"] = "video";
    msg["enabled"] = micEnabled;
    sigClient->sendJson(msg); 
}