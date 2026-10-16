qt_add_library(core STATIC
call/CallController.h
models/.gitkeep
net/.gitkeep
media/LocalCameraManager.h media/LocalCameraManager.cpp
media/AudioManager.h media/AudioManager.cpp)

target_sources(core PRIVATE
    call/CallController.cpp
    call/CallController.h
    net/SignalClient.cpp
    net/SignalClient.h
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(core PUBLIC
    Qt6::Core
    Qt6::Network
    Qt6::Multimedia
    Qt6::WebSockets
    LibDataChannel::LibDataChannel
)
