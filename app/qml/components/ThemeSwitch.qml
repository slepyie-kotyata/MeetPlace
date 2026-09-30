import QtQuick.Templates as T
import QtQuick
import "../theme/DesignTokens" as Tokens
import QtQml

T.Switch {
    enum Status { Status_Default, Status_Hovered, Status_Pressed, Status_Disabled}
    enum Check { Check_Off, Check_On}

    id: switchRoot

    property alias indicatorClip: indicator.clip
    property alias indicatorRotation: indicator.rotation
    property alias indicatorY: indicator.y
    property alias themeSwitchHeight: themeSwitch.height
    property alias themeSwitchWidth: themeSwitch.width
    property alias indicatorSource: indicator.source
    property alias themeSwitchColor: themeSwitch.color

    property int check_1: ThemeSwitch.Check.Check_Off
    property int status_1: ThemeSwitch.Status.Status_Default

    height: 16
    width: 32

    background: Rectangle {
        id: themeSwitch
    
        border.color: "white"
        border.width: 0
        color: Tokens.Light_mode.colors.primaryColor
        radius: 12
    }
    contentItem: Item {
        id: switchRootcontentItem
    }
    indicator: Item {
        id: switchRootindicator

        width: height
                height: switchRoot.height - 4
                y: 2
    
        Image {
            id: indicator
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit

            rotation: 0
            source: Qt.resolvedUrl("assets/indicator.png")
        }
    }

    states: [
        State {
            name: "Status=Default, Check=Off"
            when: switchRoot.status_1 === ThemeSwitch.Status.Status_Default && switchRoot.check_1 === ThemeSwitch.Check.Check_Off
    
            PropertyChanges {
                color: Tokens.Light_mode.colors.primaryColor
                target: themeSwitch
            }
            PropertyChanges {
                border.width: 0
                target: themeSwitch
            }
            PropertyChanges {
                border.color: "white"
                target: themeSwitch
            }
            PropertyChanges {
                x: 3
    
                target: switchRootindicator
            }
            PropertyChanges {
                source: Qt.resolvedUrl("../../assets/indicator.svg")
                target: indicator
            }
            PropertyChanges {
                rotation: 0
                target: indicator
            }
            PropertyChanges {
                clip: true
                target: indicator
            }
        },
        State {
            name: "Status=Default, Check=On"
            when: switchRoot.status_1 === ThemeSwitch.Status.Status_Default && switchRoot.check_1 === ThemeSwitch.Check.Check_On
    
            PropertyChanges {
                color: Tokens.Dark_mode.colors.primaryColor
                target: themeSwitch
            }
            PropertyChanges {
                border.width: 0
                target: themeSwitch
            }
            PropertyChanges {
                border.color: "white"
                target: themeSwitch
            }
            PropertyChanges {
                x: 25
    
                target: switchRootindicator
            }
            PropertyChanges {
                source: Qt.resolvedUrl("../../assets/indicator_7.svg")
                target: indicator
            }
            PropertyChanges {
                rotation: -0.42
                target: indicator
            }
            PropertyChanges {
                clip: true
                target: indicator
            }
        },
        State {
            name: "Status=Hovered, Check=Off"
            when: switchRoot.status_1 === ThemeSwitch.Status.Status_Hovered && switchRoot.check_1 === ThemeSwitch.Check.Check_Off
    
            PropertyChanges {
                color: Tokens.Light_mode.colors.primaryColor
                target: themeSwitch
            }
            PropertyChanges {
                border.width: 0.50
                target: themeSwitch
            }
            PropertyChanges {
                border.color: Tokens.Dark_mode.colors.primaryColor
                target: themeSwitch
            }
            PropertyChanges {
                x: 3

                target: switchRootindicator
            }
            PropertyChanges {
                source: Qt.resolvedUrl("../../assets/indicator.svg")
                target: indicator
            }
            PropertyChanges {
                rotation: 0
                target: indicator
            }
            PropertyChanges {
                clip: true
                target: indicator
            }
        },
        State {
            name: "Status=Hovered, Check=On"
            when: switchRoot.status_1 === ThemeSwitch.Status.Status_Hovered && switchRoot.check_1 === ThemeSwitch.Check.Check_On
    
            PropertyChanges {
                color: Tokens.Dark_mode.colors.primaryColor
                target: themeSwitch
            }
            PropertyChanges {
                border.width: 0.50
                target: themeSwitch
            }
            PropertyChanges {
                border.color: Tokens.Light_mode.colors.primaryColor
                target: themeSwitch
            }
            PropertyChanges {
                x: 25

                target: switchRootindicator
            }
            PropertyChanges {
                source: Qt.resolvedUrl("../../assets/indicator_5.svg")
                target: indicator
            }
            PropertyChanges {
                rotation: -0.42
                target: indicator
            }
            PropertyChanges {
                clip: true
                target: indicator
            }
        },
        State {
            name: "Status=Pressed, Check=Off"
            when: switchRoot.status_1 === ThemeSwitch.Status.Status_Pressed && switchRoot.check_1 === ThemeSwitch.Check.Check_Off
    
            PropertyChanges {
                color: Tokens.Light_mode.colors.primaryColor
                target: themeSwitch
            }
            PropertyChanges {
                border.width: 0
                target: themeSwitch
            }
            PropertyChanges {
                border.color: "white"
                target: themeSwitch
            }
            PropertyChanges {
                x: 3

                target: switchRootindicator
            }
            PropertyChanges {
                source: Qt.resolvedUrl("../../assets/indicator_4.svg")
                target: indicator
            }
            PropertyChanges {
                rotation: 0
                target: indicator
            }
            PropertyChanges {
                clip: true
                target: indicator
            }
        },
        State {
            name: "Status=Pressed, Check=On"
            when: switchRoot.status_1 === ThemeSwitch.Status.Status_Pressed && switchRoot.check_1 === ThemeSwitch.Check.Check_On
    
            PropertyChanges {
                color: Tokens.Dark_mode.colors.primaryColor
                target: themeSwitch
            }
            PropertyChanges {
                border.width: 0
                target: themeSwitch
            }
            PropertyChanges {
                border.color: "white"
                target: themeSwitch
            }
            PropertyChanges {
                x: 25

                target: switchRootindicator
            }
            PropertyChanges {
                source: Qt.resolvedUrl("../../assets/indicator_3.svg")
                target: indicator
            }
            PropertyChanges {
                rotation: -0.42
                target: indicator
            }
            PropertyChanges {
                clip: true
                target: indicator
            }
        },
        State {
            name: "Status=Disabled, Check=Off"
            when: switchRoot.status_1 === ThemeSwitch.Status.Status_Disabled && switchRoot.check_1 === ThemeSwitch.Check.Check_Off
    
            PropertyChanges {
                color: "#cdcdcd"
                target: themeSwitch
            }
            PropertyChanges {
                border.width: 0
                target: themeSwitch
            }
            PropertyChanges {
                border.color: "white"
                target: themeSwitch
            }
            PropertyChanges {
                x: 2

                target: switchRootindicator
            }
            PropertyChanges {
                source: Qt.resolvedUrl("../../assets/indicator_2.png")
                target: indicator
            }
            PropertyChanges {
                rotation: 0
                target: indicator
            }
            PropertyChanges {
                clip: true
                target: indicator
            }
        },
        State {
            name: "Status=Disabled, Check=On"
            when: switchRoot.status_1 === ThemeSwitch.Status.Status_Disabled && switchRoot.check_1 === ThemeSwitch.Check.Check_On
    
            PropertyChanges {
                color: "#e3e3e3"
                target: themeSwitch
            }
            PropertyChanges {
                border.width: 0
                target: themeSwitch
            }
            PropertyChanges {
                border.color: "white"
                target: themeSwitch
            }
            PropertyChanges {
                x: 25

                target: switchRootindicator
            }
            PropertyChanges {
                source: Qt.resolvedUrl("../../assets/indicator_1.png")
                target: indicator
            }
            PropertyChanges {
                rotation: 0
                target: indicator
            }
            PropertyChanges {
                clip: true
                target: indicator
            }
        }
    ]

    Binding {
        property: "status_1"
        target: switchRoot
        value: !switchRoot.enabled ? ThemeSwitch.Status.Status_Disabled
            : switchRoot.pressed ? ThemeSwitch.Status.Status_Pressed
            : switchRoot.hovered ? ThemeSwitch.Status.Status_Hovered
            : ThemeSwitch.Status.Status_Default
    }
    Binding {
        property: "check_1"
        target: switchRoot
        value: switchRoot.checked ? ThemeSwitch.Check.Check_On
            : ThemeSwitch.Check.Check_Off
    }
}