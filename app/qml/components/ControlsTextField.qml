import QtQuick.Templates as T
import QtQuick
import "../theme/DesignTokens" as Tokens
import QtQml

T.TextField {
    enum Status { Status_Default, Status_Hovered, Status_ActiveFocus, Status_Disabled}

    id: textFieldRoot

    property alias controlsTextFieldBorderColor: controlsTextField.border.color
    property alias controlsTextFieldHeight: controlsTextField.height
    property alias controlsTextFieldPlaceholder: textFieldRoot.placeholder
    property alias controlsTextFieldWidth: controlsTextField.width
    property alias textFontNodeFontPixelSize: textFontNode.font.pixelSize
    property alias textFontNodeHeight: textFontNode.height
    property alias textFontNodeWidth: textFontNode.width
    property alias textFontNodeY: textFontNode.y
    property alias textFontNodeColor: textFontNode.color

    property string placeholder: "User Input"
    property int status_1: ControlsTextField.Status.Status_Default

    height: 32
    width: 180

    color: textFontNode.color
    font: textFontNode.font
    leftPadding: textFontNode.x
    topPadding: textFontNode.y

    background: Rectangle {
        id: controlsTextField

        border.color: "#e4e4e7"
        border.width: 1
        color: "transparent"
        radius: 12

        Text {
            id: textFontNode

            x: 8
            y: 8

            height: 15
            width: 60

            color: Tokens.Light_mode.colors.subtextColor
            font.family: "Inter"
            font.pixelSize: 12
            font.weight: Font.Normal
            horizontalAlignment: Text.AlignLeft
            text: textFieldRoot.placeholder
            verticalAlignment: Text.AlignTop
            visible: textFieldRoot.text === ""
        }
    }

    states: [
        State {
            name: "Status=Default"
            when: textFieldRoot.status_1 === ControlsTextField.Status.Status_Default

            PropertyChanges {
                border.color: "#e4e4e7"
                target: controlsTextField
            }
            PropertyChanges {
                color: "transparent"
                target: controlsTextField
            }
            PropertyChanges {
                color: Tokens.Light_mode.colors.subtextColor
                target: textFontNode
            }
        },
        State {
            name: "Status=Hovered"
            when: textFieldRoot.status_1 === ControlsTextField.Status.Status_Hovered

            PropertyChanges {
                border.color: "#595959"
                target: controlsTextField
            }
            PropertyChanges {
                color: "transparent"
                target: controlsTextField
            }
            PropertyChanges {
                color: Tokens.Light_mode.colors.subtextColor
                target: textFontNode
            }
        },
        State {
            name: "Status=ActiveFocus"
            when: textFieldRoot.status_1 === ControlsTextField.Status.Status_ActiveFocus

            PropertyChanges {
                border.color: "#e3e3e3"
                target: controlsTextField
            }
            PropertyChanges {
                color: "transparent"
                target: controlsTextField
            }
            PropertyChanges {
                color: Tokens.Light_mode.colors.subtextColor
                target: textFontNode
            }
        },
        State {
            name: "Status=Disabled"
            when: textFieldRoot.status_1 === ControlsTextField.Status.Status_Disabled

            PropertyChanges {
                border.color: "#3f3f3f"
                target: controlsTextField
            }
            PropertyChanges {
                color: "#b4b3b3"
                target: controlsTextField
            }
            PropertyChanges {
                color: "#595959"
                target: textFontNode
            }
        }
    ]

    Binding {
        property: "status_1"
        target: textFieldRoot
        value: !textFieldRoot.enabled ? ControlsTextField.Status.Status_Disabled
            : textFieldRoot.activeFocus ? ControlsTextField.Status.Status_ActiveFocus
            : textFieldRoot.hovered ? ControlsTextField.Status.Status_Hovered
            : ControlsTextField.Status.Status_Default
    }
}