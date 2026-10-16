#ifndef SIGNALCLIENT_H
#define SIGNALCLIENT_H
#include <QObject>
#include <QWebSocket>

class SignalClient : public QObject {
    Q_OBJECT
public:
    SignalClient(QObject *parent = nullptr);
    void connectToServer(const QUrl &url);
    void disconnectFromServer();
    void sendJson(const QJsonObject &json);
signals:
    void socketConnected();
    void socketDisconnected();
    void socketError(const QString &errorString);
    void messageReceived(const QJsonObject &message);

private slots:
    void onConnected();
    void onTextMessageReceived(const QString &message);

private:
    QWebSocket p_webSocket;
};
#endif