qt_add_executable(appMeetPlace
    main.cpp
)

set_source_files_properties(
    qml/theme/DesignTokens/Light_mode.qml
    qml/theme/DesignTokens/Dark_mode.qml
    PROPERTIES QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(appMeetPlace
    URI MeetPlace
    QML_FILES
        qml/Main.qml
        qml/screens/MainAppWindow.qml
        qml/components/ControlsTextField.qml
        qml/components/MainButton_Light.qml
        qml/components/ThemeSwitch.qml
        qml/theme/DesignTokens/Dark_mode.qml
        qml/theme/DesignTokens/Dark_modeColors.qml
        qml/theme/DesignTokens/Dark_modeTheme.qml
        qml/theme/DesignTokens/Light_mode.qml
        qml/theme/DesignTokens/Light_modeColors.qml
        qml/theme/DesignTokens/Light_modeTheme.qml
    RESOURCES
        assets/indicator.svg
        assets/indicator_1.png
        assets/indicator_2.png
        assets/indicator_3.svg
        assets/indicator_4.svg
        assets/indicator_5.svg
        assets/indicator_7.svg
        assets/indicator_8.svg
        # По мере создания сюда добавлются новые интерфейсы:
        # qml/screens/StartScreen.qml
        # qml/components/VideoTile.qml
        # qml/theme/Theme.qml
)

set_target_properties(appMeetPlace PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER ru.enjine.meetplace
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appMeetPlace PRIVATE
    Qt6::Quick
    Qt6::VirtualKeyboard
    core
)

if (TARGET importedcontent)
    target_link_libraries(appMeetPlace PRIVATE importedcontent)
endif()

install(TARGETS appMeetPlace
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
