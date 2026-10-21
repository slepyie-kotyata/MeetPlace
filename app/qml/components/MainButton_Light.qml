import QtQuick.Templates as T
import QtQuick
import "../theme/DesignTokens" as Tokens
import QtQml

T.Button {
    enum Status { Status_Default, Status_Hovered, Status_Pressed, Status_Disabled}

    id: buttonRoot

    property alias labelX: label.x
    property alias labelY: label.y
    property alias mainButton_LightHeight: mainButton_Light.height
    property alias mainButton_LightLabelText: buttonRoot.labelText
    property alias mainButton_LightWidth: mainButton_Light.width
    property alias labelColor: label.color
    property alias mainButton_LightColor: mainButton_Light.color
    property alias labelWidth: label.width
    property color hoverBorderColor: isDarkMode
        ? Tokens.Dark_mode.colors.accentColor
        : Tokens.Light_mode.colors.accentColor

    property bool isDarkMode: false

    property string labelText: "Button"
    property int status_1: MainButton_Light.Status.Status_Default

    height: 40
    width: 86

    background: Rectangle {
        id: mainButton_Light

        border.color: isDarkMode ? Tokens.Light_mode.colors.primaryColor : Tokens.Dark_mode.colors.primaryColor
        border.width: 0
        color: mainButton_LightColor
        radius: 4
        Rectangle {
                    anchors.fill: parent
                    radius: parent.radius
                    color: "#000000"
                    opacity: buttonRoot.status_1 === MainButton_Light.Status.Status_Pressed ? 0.1 : 0.0
                    Behavior on opacity { NumberAnimation { duration: 100 } }
                }

        Behavior on color { ColorAnimation { duration: 200 } }
        Behavior on border.width { NumberAnimation { duration: 150 } }
        Behavior on border.color { ColorAnimation { duration: 150 } }
    }

    contentItem: Item {
        id: buttonRootcontentItem

        Text {
            id: label

            x: 20
            y: 11.50

            height: 17
            width: buttonRoot.width - (x * 2)

            color: labelColor
            font.family: "Inter"
            font.pixelSize: 14

            horizontalAlignment: Text.AlignLeft
            text: buttonRoot.labelText
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.NoWrap
            Behavior on color { ColorAnimation { duration: 200 } }
        }
    }
    states: [
            State {
                name: "Status=Default"
                when: buttonRoot.status_1 === MainButton_Light.Status.Status_Default
                PropertyChanges {
                                target: mainButton_Light
                                border.width: 0
                                border.color: isDarkMode ? Tokens.Dark_mode.colors.primaryColor : Tokens.Light_mode.colors.primaryColor
                            }
            },
            State {
                name: "Status=Hovered"
                when: buttonRoot.status_1 === MainButton_Light.Status.Status_Hovered
                PropertyChanges {
                    target: mainButton_Light
                    border.width: 1
                    border.color: buttonRoot.hoverBorderColor
                }
            },
            State {
                name: "Status=Pressed"
                when: buttonRoot.status_1 === MainButton_Light.Status.Status_Pressed
                PropertyChanges {
                    target: mainButton_Light
                    border.width: 1
                    border.color: buttonRoot.hoverBorderColor
                }
            },
            State {
                name: "Status=Disabled"
                when: buttonRoot.status_1 === MainButton_Light.Status.Status_Disabled
                PropertyChanges {
                    target: mainButton_Light
                    border.width: 0
                    color: "#e3e3e3"
                }
            }
        ]

    Binding {
        target: buttonRoot
        property: "status_1"
        value: !buttonRoot.enabled
            ? MainButton_Light.Status.Status_Disabled
            : buttonRoot.pressed
                ? MainButton_Light.Status.Status_Pressed
                : buttonRoot.hovered
                    ? MainButton_Light.Status.Status_Hovered
                    : MainButton_Light.Status.Status_Default
    }
    }