qt_add_executable(call-sandbox
    main.cpp
)

target_link_libraries(call-sandbox PRIVATE
    Qt6::Multimedia
    Qt6::WebSockets
    core
)