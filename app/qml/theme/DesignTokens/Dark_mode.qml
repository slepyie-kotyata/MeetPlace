pragma Singleton

import QtQuick

QtObject {
    id: dark_mode

    property Dark_modeTheme activeTheme: mode_1
    property Dark_modeColors colors: activeTheme.colors

    property Dark_modeTheme mode_1: Dark_modeTheme {
        colors: Dark_modeColors {
            accentColor: "#fdb892"
            backgroundColor: "#121214"
            borderColor: "#2f2f32"
            dangerColor: "#ef5350"
            panelColor: "#1e1e22"
            primaryColor: "#c2a4d6"
            subtextColor: "#a1a1aa"
            textColor: "#f0f0f5"
        }
    }
}