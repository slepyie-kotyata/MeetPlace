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

    Row {
        anchors { left: parent.left; bottom: parent.bottom; margins: 44 }
        spacing: 8

        ComboBox {
            model: AudioManager.audioInputs.map(d => d.name + (d.isDefault ? " (default)" : ""))
            onActivated: (i) => AudioManager.selectInput(AudioManager.audioInputs[i].id)
        }
        ComboBox {
            model: AudioManager.audioOutputs.map(d => d.name + (d.isDefault ? " (default)" : ""))
            onActivated: (i) => AudioManager.selectOutput(AudioManager.audioOutputs[i].id)
        }
        Button {
            text: AudioManager.metering ? "Stop" : "Start"
            onClicked: AudioManager.metering ? AudioManager.stopMeter() : AudioManager.startMeter()
        }
        Rectangle {
            width: 120; height: 12; radius: 6; color: "#333"
            Rectangle {
                width: parent.width * AudioManager.inputLevel
                height: parent.height; radius: 6; color: "#4cd964"
            }
        }
    }

    Component.onCompleted: {
        LocalCameraManager.videoSink = output.videoSink
        LocalCameraManager.start()
    }
}