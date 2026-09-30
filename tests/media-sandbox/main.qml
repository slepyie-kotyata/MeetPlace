import QtQuick
import QtQuick.Controls
import MeetPlace.Core
import QtMultimedia

ApplicationWindow {
    width: 960; height: 720; visible: true
    title: "Media Sandbox — Dev2"

    VideoOutput {
        id: output
        anchors.fill: parent
    }

    Row {
        anchors { left: parent.left; bottom: parent.bottom; margins: 12 }
        spacing: 8

        ComboBox {
            model: LocalCameraManager.cameras.map(c => c.name)
            onActivated: (i) => LocalCameraManager.start(LocalCameraManager.cameras[i].id)
        }
        Button {
            text: "Stop"
            onClicked: LocalCameraManager.stop()
        }
        Label {
            text: LocalCameraManager.active ? "● live" : "○ idle"
        }
    }

    Component.onCompleted: {
        LocalCameraManager.videoSink = output.videoSink
        LocalCameraManager.start()
    }
}