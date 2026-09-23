qt_add_executable(appMeetPlace
    main.cpp
)

qt_add_qml_module(appMeetPlace
    URI MeetPlace
    QML_FILES
        qml/Main.qml
        RESOURCES qml/components/.gitkeep
        RESOURCES qml/screens/.gitkeep
        RESOURCES qml/theme/.gitkeep
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
