
import QtQuick.Templates as T
import QtQuick
import "../theme/DesignTokens" as Tokens
import MeetPlace

T.Button {
    id: controlsButton

    enum Status {
        Status_Default,
        Status_Hovered,
        Status_Pressed,
        Status_Disabled
    }

    property alias controlBackgroundBorderColor: controlBackground.border.color
    property alias controlBackgroundBorderWidth: controlBackground.border.width
    property alias buttonColor: controlBackground.color

    property string iconType: "settings"
    property alias iconSource: iconImage.source

    property bool isDarkMode: false

    readonly property color hoverBorderColor: isDarkMode
        ? Tokens.Dark_mode.colors.accentColor
        : Tokens.Light_mode.colors.accentColor

    readonly property int currentStatus:
        !enabled ? ControlsButton.Status.Status_Disabled
        : pressed ? ControlsButton.Status.Status_Pressed
        : hovered ? ControlsButton.Status.Status_Hovered
        : ControlsButton.Status.Status_Default

    height: 40
    width: 40
    enabled: true

    background: Rectangle {
        id: controlBackground

        color: controlsButton.isDarkMode
            ? Tokens.Dark_mode.colors.primaryColor
            : Tokens.Light_mode.colors.primaryColor

        border.width: 0
        border.color: controlsButton.isDarkMode
            ? Tokens.Dark_mode.colors.primaryColor
            : Tokens.Light_mode.colors.primaryColor

        radius: 4

        Rectangle {
            anchors.fill: parent
            radius: parent.radius
            color: "#000000"
            opacity: controlsButton.currentStatus
                     === ControlsButton.Status.Status_Pressed ? 0.1 : 0.0

            Behavior on opacity {
                NumberAnimation { duration: 100 }
            }
        }

        Behavior on color {
            ColorAnimation { duration: 200 }
        }

        Behavior on border.width {
            NumberAnimation { duration: 150 }
        }

        Behavior on border.color {
            ColorAnimation { duration: 150 }
        }
    }

    contentItem: Item {
        Loader {
            anchors.centerIn: parent
            source: controlsButton.iconType === "settings"
                ? "Icon_Settings.qml"
                : "Icon_Placeholder.qml"
        }

        Image {
            id: iconImage
            anchors.centerIn: parent
            width: 24
            height: 24
            fillMode: Image.PreserveAspectFit
            visible: source !== ""
        }
    }

    states: [
        State {
            name: "Status=Default"
            when: controlsButton.currentStatus
                  === ControlsButton.Status.Status_Default

            PropertyChanges {
                target: controlBackground
                border.width: 0
                border.color: controlsButton.isDarkMode
                    ? Tokens.Dark_mode.colors.primaryColor
                    : Tokens.Light_mode.colors.primaryColor
            }
        },

        State {
            name: "Status=Hovered"
            when: controlsButton.currentStatus
                  === ControlsButton.Status.Status_Hovered

            PropertyChanges {
                target: controlBackground
                border.width: 1
                border.color: controlsButton.hoverBorderColor
            }
        },

        State {
            name: "Status=Pressed"
            when: controlsButton.currentStatus
                  === ControlsButton.Status.Status_Pressed

            PropertyChanges {
                target: controlBackground
                border.width: 1
                border.color: controlsButton.hoverBorderColor
            }
        },

        State {
            name: "Status=Disabled"
            when: controlsButton.currentStatus
                  === ControlsButton.Status.Status_Disabled

            PropertyChanges {
                target: controlBackground
                border.width: 0
                border.color: "#e3e3e3"
                color: "#e3e3e3"
            }
        }
    ]
}
