import QtQuick
import QtQuick.Shapes
import "../theme/DesignTokens" as Tokens

Rectangle {
    id: controlsButton_Icon_Placeholder

    height: 30
    width: 30

    color: "transparent"

    property color iconColor: Tokens.Light_mode.colors.panelColor

    Shape {
        id: _vector

        height: 30
        width: 30

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        ShapePath {
            id: _vector_ShapePath0

            fillColor: "#00000000"
            fillRule: ShapePath.WindingFill
            strokeColor: controlsButton_Icon_Placeholder.iconColor
            strokeWidth: 1

            PathSvg {
                id: _vector_ShapePath0_PathSvg0

                path: "M23.3884 8.2618C23.3884 12.5485 19.8567 16.0236 15.5 16.0236C11.1434 16.0236 7.6117 12.5485 7.6117 8.2618C7.6117 3.97508 11.1434 0.5 15.5 0.5C19.8567 0.5 23.3884 3.97508 23.3884 8.2618ZM30.5 30.5C29.481 27.3735 27.4793 24.6461 24.7835 22.7111C22.0876 20.7761 18.8369 19.7334 15.5 19.7334C12.1631 19.7334 8.91239 20.7761 6.21654 22.7111C3.5207 24.6461 1.51899 27.3735 0.5 30.5H30.5Z"
            }
        }
    }
}