#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include "call/CallController.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    
    CallController controller;
    QObject::connect(&controller, &CallController::stateChanged, [&controller]() {
        qDebug() << "[STATE]: State changed to:" << controller.getState();
    });
    QObject::connect(&controller, &CallController::peerJoined, [](const QString &peerId, const QString &name) {
        qDebug() << "[EVENT]: Peer joined" << name << "(ID:" << peerId << ")";
    });
    QObject::connect(&controller, &CallController::peerLeft, [](const QString &peerId) {
        qDebug() << "[EVENT]: Peer left ID:" << peerId;
    });
    QObject::connect(&controller, &CallController::errorOccurred, [](const QString &code, const QString &msg) {
        qDebug() << "[ERROR]:" << code << "-" << msg;
    });
    
    QTimer::singleShot(100, [&controller]() {
        qDebug() << "Отправляем запрос на вход в комнату 'test-room' под id 'user1'...";
        controller.joinCall("test-room", "user1");
    });
    
    QTimer::singleShot(10000, [&controller, &app]() {
        qDebug() << "Время вышло. Покидаем комнату...";
        controller.leaveCall();
        app.quit();
    });
    
    return app.exec();
};