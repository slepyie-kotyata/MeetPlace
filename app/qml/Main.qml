import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import "screens"
import QtQuick.VirtualKeyboard

ApplicationWindow {
    id: window
    width: 640
    height: 480
    minimumWidth: 200
    minimumHeight: 250
    visible: true
    title: qsTr("MeetPlace")
    property bool lightMode: Application.styleHints.colorScheme === Qt.Light
    property color reallyDark: "#1f1f1f"
    property color dark: "#262626"
    property color reallyLight: "#e7e7e7"
    property color light: "#e0e0e0"
    property bool isDarkMode: false
    Loader {
            id: screenLoader
            anchors.fill: parent
            source: "screens/MainAppWindow.qml"

            onItemChanged: {
                if (item) {
                    if (item.hasOwnProperty("isDarkMode")) {
                        item.isDarkMode = window.isDarkMode

                        item.isDarkModeChanged.connect(function() {
                            window.isDarkMode = item.isDarkMode
                        })
                    }

                    if (item && item.loginRequested) {
                        item.loginRequested.connect(function() {
                            screenLoader.source = "screens/HomeAppWindow.qml"
                        })
                    }
                }
            }
        }

    onIsDarkModeChanged: {
            if (screenLoader.item && screenLoader.item.hasOwnProperty("isDarkMode")) {
                screenLoader.item.isDarkMode = window.isDarkMode
            }
        }

    InputPanel {
        id: inputPanel
        z: 99
        y: window.height
        width: window.width

        states: State {
            name: "visible"
            when: inputPanel.active
            PropertyChanges {
                inputPanel.y: window.height - inputPanel.height
            }
        }
        transitions: Transition {
            from: ""
            to: "visible"
            reversible: true
            NumberAnimation {
                properties: "y"
                easing.type: Easing.InOutQuad
            }
        }
    }
}
