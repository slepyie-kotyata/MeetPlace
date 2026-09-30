qt_add_executable(media-sandbox
    main.cpp
)

qt_add_qml_module(media-sandbox
    URI MediaSandbox
    VERSION 1.0
    QML_FILES Main.qml
)

set_target_properties(media-sandbox PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER ru.enjine.sandbox
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(APPLE AND NOT IOS)
    set_target_properties(media-sandbox PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/macos/Info.plist.in"
    )
elseif(IOS)
    set_target_properties(media-sandbox PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/ios/Info.plist.in"
    )
endif()

target_link_libraries(media-sandbox PRIVATE
    Qt6::Quick
    Qt6::Multimedia
    core
)