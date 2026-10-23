#ifndef SIGNALCLIENT_H
#define SIGNALCLIENT_H
#include <QObject>
#include <QWebSocket>

class SignalClient : public QObject {
    Q_OBJECT
signals:
    void socketConnected();
    void socketDisconnected();
    void socketError(const QString &errorString);
    void messageReceived(const QJsonObject &message);
public:
    SignalClient(QObject *parent = nullptr);
    void connectToServer(const QUrl &url);
    void disconnectFromServer();
    void sendJson(const QJsonObject &json);
private slots:
    void onMessageReceived(const QString &message);
private:
    QWebSocket p_webSocket;
};
#endif