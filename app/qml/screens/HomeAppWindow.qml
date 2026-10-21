import QtQuick
import "../components"
import "../theme/DesignTokens" as Tokens

Rectangle {
    id: homeAppWindow

    property bool isDarkMode: false

    anchors.fill: parent

    clip: true
    readonly property var currentTokens: isDarkMode ? Tokens.Dark_mode : Tokens.Light_mode

    color: currentTokens.colors.backgroundColor

    Rectangle {
        id: authCard

        height: 223
        width: 400
        anchors.centerIn: parent
        color: currentTokens.colors.panelColor
        radius: 12

        Text {
            id: titleText

            height: 29
            width: 197

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: 0.50
            anchors.top: parent.top
            anchors.topMargin: 35

            color: currentTokens.colors.textColor
            font.family: "Inter"
            font.pixelSize: 24
            font.weight: Font.Normal
            horizontalAlignment: Text.AlignLeft
            text: qsTr("Начать общение")
            textFormat: Text.PlainText
            verticalAlignment: Text.AlignTop
        }
        MainButton_Light {
            id: mainButton_Light
            isDarkMode: homeAppWindow.isDarkMode

            x: 40
            y: 144

            height: 41
            width: 320
            labelText: "Создать встречу"
            font.weight: Font.Normal
            labelX: 16
            labelY: 12.50
            mainButton_LightHeight: 41
            mainButton_LightWidth: 320

            mainButton_LightColor: currentTokens.colors.primaryColor
            labelColor: currentTokens.colors.textColor
            status_1: MainButton_Light.Status.Status_Default
        }
        MainButton_Light {
            id: mainButton_Light_1
            isDarkMode: homeAppWindow.isDarkMode

            x: 40
            y: 84

            height: 41
            width: 320
            labelText: "Найти встречу"
            font.weight: Font.Normal
            labelX: 16
            labelY: 12.50
            mainButton_LightHeight: 41
            mainButton_LightWidth: 320

            mainButton_LightColor: currentTokens.colors.primaryColor
            labelColor: currentTokens.colors.textColor
        }
        Rectangle {
            id: frame_2

            height: 1
            width: 1

            anchors.left: parent.left
            anchors.leftMargin: 98
            anchors.top: parent.top
            anchors.topMargin: 166

            clip: true
            color: "transparent"
        }
    }
    Rectangle {
        id: frame_1

        height: parent.height
        width: 94

        anchors.right: parent.right
        anchors.top: parent.top

        clip: true
        color: currentTokens.colors.panelColor

        ThemeSwitch {
            id: themeSwitch

            x: 23
            y: 24

            height: 24
            width: 48

            checked: homeAppWindow.isDarkMode

            MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                                    homeAppWindow.isDarkMode = !homeAppWindow.isDarkMode
                        }
            }
        }
        ControlsButton {
            id: controlsButton
            x: 27
            y: 68

            width: 40
            height: 40

            isDarkMode: homeAppWindow.isDarkMode
            buttonColor: homeAppWindow.currentTokens.colors.primaryColor
            iconType: "settings"
        }

        ControlsButton {
            id: controlsButton_1
            x: 27
            y: 128

            width: 40
            height: 40

            isDarkMode: homeAppWindow.isDarkMode
            buttonColor: homeAppWindow.currentTokens.colors.primaryColor
            iconType: "profile"
        }
    }

}