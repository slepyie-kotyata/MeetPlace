pragma Singleton

import QtQuick

QtObject {
    id: light_mode

    property Light_modeTheme activeTheme: mode_1
    property Light_modeColors colors: activeTheme.colors

    property Light_modeTheme mode_1: Light_modeTheme {
        colors: Light_modeColors {
            accentColor: "#c2a4d6"
            backgroundColor: "#f8f9fa"
            borderColor: "#e4e4e7"
            dangerColor: "#e53935"
            panelColor: "#ffffff"
            primaryColor: "#fdb892"
            subtextColor: "#71717a"
            textColor: "#1a1a1e"
        }
    }
}