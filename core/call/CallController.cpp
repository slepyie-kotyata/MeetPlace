#include "CallController.h"
#include <qobject.h>

void CallController::joinRoom(const QString &roomId, const QString &name) {};
void CallController::leaveRoom() {};
void CallController::toggleMute() {};
void CallController::toggleCamera() {};
void CallController::startScreenShare() {};
void CallController::stopScreenShare() {};
void CallController::sendChatMessage(const QString &text) {};

// остальное потом (может быть оно и не понадобится)