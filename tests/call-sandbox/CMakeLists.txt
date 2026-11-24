qt_add_executable(call-sandbox
    main.cpp
)

target_link_libraries(call-sandbox PRIVATE
    Qt6::Gui
    Qt6::Multimedia
    Qt6::WebSockets
    core
)

set_target_properties(call-sandbox PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER ru.enjine.call-sandbox
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(APPLE AND NOT IOS)
    set_target_properties(call-sandbox PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/macos/Info.plist.in"
    )
endif()
