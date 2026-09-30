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

    property string labelText: "Button"
    property int status_1: MainButton_Light.Status.Status_Default

    height: 40
    width: 86

    background: Rectangle {
        id: mainButton_Light

        border.color: "white"
        border.width: 0
        color: Tokens.Light_mode.colors.primaryColor
        radius: 4
    }

    contentItem: Item {
        id: buttonRootcontentItem

        Text {
            id: label

            x: 20
            y: 11.50

            height: 17
            width: 47

            color: Tokens.Light_mode.colors.textColor
            font.family: "Inter"
            font.pixelSize: 14
            font.weight: Font.DemiBold
            horizontalAlignment: Text.AlignHCenter
            text: buttonRoot.labelText
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.Wrap
        }
    }

    Binding {
        property: "status_1"
        target: buttonRoot
        value: !buttonRoot.enabled ? MainButton_Light.Status.Status_Disabled
            : buttonRoot.pressed ? MainButton_Light.Status.Status_Pressed
            : buttonRoot.hovered ? MainButton_Light.Status.Status_Hovered
            : MainButton_Light.Status.Status_Default
    }
}