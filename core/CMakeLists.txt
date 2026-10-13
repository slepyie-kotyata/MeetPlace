qt_add_library(core STATIC
call/.gitkeep
models/.gitkeep
net/.gitkeep
media/LocalCameraManager.h media/LocalCameraManager.cpp
media/AudioManager.h media/AudioManager.cpp)

target_sources(core PRIVATE
    # Пример заполнения для будущих файлов:
    # call/CallController.cpp
    # call/CallController.h
    # net/SignalClient.cpp
    # net/SignalClient.h
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Qt6 REQUIRED COMPONENTS Network Multimedia WebSockets)
find_package(LibDataChannel REQUIRED)

target_link_libraries(core PUBLIC
    Qt6::Network
    Qt6::Multimedia
    Qt6::WebSockets
    LibDataChannel::LibDataChannel
)
