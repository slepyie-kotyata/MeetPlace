#include "SignalClient.h"
#include <QObject>
#include <QJsonObject>

SignalClient::SignalClient(QObject *parent) : QObject(parent){
    connect(&p_webSocket, &QWebSocket::connected, this, &SignalClient::onConnected);
    connect(&p_webSocket, &QWebSocket::disconnected, this, &SignalClient::socketDisconnected);
    connect(&p_webSocket, &QWebSocket::errorOccurred, this, [this](QAbstractSocket::SocketError) {
        emit socketError(p_webSocket.errorString());
    }); //реакция на ошибку, парсить json не надо, используем лямбду-функцию
    connect(&p_webSocket, &QWebSocket::textMessageReceived, this, &SignalClient::onTextMessageReceived);
};

void SignalClient::connectToServer(const QUrl &url) {
    p_webSocket.open(url);
};

void SignalClient::disconnectFromServer() {
    p_webSocket.close();  
};

void SignalClient::sendJson(const QJsonObject &json) {
    QJsonDocument doc(json);
    p_webSocket.sendTextMessage(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
};

void SignalClient::onConnected() {
    emit socketConnected(); //испускаем сигнал
};

void SignalClient::onTextMessageReceived(const QString &message) {
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (doc.isObject()) {
        emit messageReceived(doc.object());
    }
};