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
    Offer,
    Ice,
    Answer,
    Error
};

static const QHash<QString, MessageType> typeMap = {
    {"joined", MessageType::Joined},
    {"peer-joined", MessageType::PeerJoined},
    {"peer-left", MessageType::PeerLeft},
    {"offer", MessageType::Offer},
    {"answer", MessageType::Answer},
    {"ice", MessageType::Ice},
    {"error", MessageType::Error},
};

CallController::CallController(QObject *parent) 
    : QObject(parent), p_state(Idle)
{
    p_sclient = new SignalClient(this);
    connect(p_sclient, &SignalClient::socketDisconnected, this, [this]() {
        setState(Idle);
    });
    connect(p_sclient, &SignalClient::socketError, this, &CallController::handleError);
    connect(p_sclient, &SignalClient::messageReceived, this, &CallController::handleMessage);
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
            emit stateChanged();
            break;
        case MessageType::PeerJoined:
            emit participantJoined(peerId, name);
            break;
        case MessageType::PeerLeft:
            emit participantLeft(peerId);
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
    emit stateChanged();
};

void CallController::joinCall(const QString &roomId, const QString &peerId) {
    setState(Connecting);

    QUrl url(WS_URL);
    QUrlQuery query;
    query.addQueryItem("room_id", roomId);
    query.addQueryItem("peer_id", peerId);
    url.setQuery(query);
    
    p_sclient->connectToServer(url);
};

void CallController::leaveCall() {
    p_sclient->disconnectFromServer();
    setState(Idle);
};

CallController::State CallController::getState() const {
    return p_state;
};

void CallController::setState(State state) {
    if (p_state != state) {
        p_state = state;
        emit stateChanged();
    }
};