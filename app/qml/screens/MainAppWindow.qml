import QtQuick
import "../components"
import "../theme/DesignTokens" as Tokens

Rectangle {
    id: mainAppWindow

    property bool isDarkMode: false

    anchors.fill: parent

    clip: true

    readonly property var currentTokens: mainAppWindow.isDarkMode ? Tokens.Dark_mode : Tokens.Light_mode

    color: currentTokens.colors.backgroundColor

    Behavior on color { ColorAnimation { duration: 200 } }

    Rectangle {
        id: authCard

        anchors.centerIn: parent

        height: 302
        width: 400

        color: mainAppWindow.isDarkMode ? Tokens.Dark_mode.colors.panelColor : Tokens.Light_mode.colors.panelColor
        radius: 12

        Behavior on color { ColorAnimation { duration: 200 } }

        MainButton_Light {
            id: loginButton
            x: 40
            y: 226

            height: 41
            width: 320

            labelText: "Вход"
            labelX: 137
            labelY: 12.50
            mainButton_LightHeight: 41
            mainButton_LightWidth: 320
            mainButton_LightColor: currentTokens.colors.primaryColor
            labelColor: currentTokens.colors.textColor
            status_1: MainButton_Light.Status.Status_Default
        }
        ControlsTextField {
            id: controlsTextField

            x: 40
            y: 86

            height: 48
            width: 320

            controlsTextFieldBorderColor: currentTokens.colors.borderColor
            controlsTextFieldHeight: 48
            controlsTextFieldPlaceholder: "Почта"
            controlsTextFieldWidth: 320
            placeholder: "Почта"
            status_1: ControlsTextField.Status.Status_Default
            textFontNodeColor: currentTokens.colors.subtextColor
            textFontNodeFontPixelSize: 14
            textFontNodeHeight: 17
            textFontNodeWidth: 42
            textFontNodeY: 16
        }
        ControlsTextField {
            id: controlsTextField_1

            x: 40
            y: 156

            height: 48
            width: 320

            controlsTextFieldBorderColor: currentTokens.colors.borderColor
            controlsTextFieldHeight: 48
            controlsTextFieldPlaceholder: "Пароль"
            controlsTextFieldWidth: 320
            placeholder: "Пароль"
            status_1: ControlsTextField.Status.Status_Default
            textFontNodeColor: currentTokens.colors.subtextColor
            textFontNodeFontPixelSize: 14
            textFontNodeHeight: 17
            textFontNodeWidth: 52
            textFontNodeY: 16
        }
        Text {
            id: titleText

            x: 123
            y: 35

            height: 29
            width: 155

            color: currentTokens.colors.textColor
            font.family: "Inter"
            font.pixelSize: 24
            font.weight: Font.Normal
            horizontalAlignment: Text.AlignLeft
            text: qsTr("Авторизация")
            textFormat: Text.PlainText
            verticalAlignment: Text.AlignTop
        }
    }
    ThemeSwitch {
            id: themeSwitch

            height: 24
            width: 48

            anchors.top: parent.top
            anchors.right: parent.right
            anchors.topMargin: 24
            anchors.rightMargin: 24

            checked: mainAppWindow.isDarkMode

            themeSwitchColor: currentTokens.colors.primaryColor

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    mainAppWindow.isDarkMode = !mainAppWindow.isDarkMode
                }
            }
        }
}