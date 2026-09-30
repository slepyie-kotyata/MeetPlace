qt_add_library(core STATIC
call/.gitkeep
models/.gitkeep
net/.gitkeep
media/LocalCameraManager.h media/LocalCameraManager.cpp)

target_sources(core PRIVATE
    # Пример заполнения для будущих файлов:
    # call/CallController.cpp
    # call/CallController.h
    # net/SignalClient.cpp
    # net/SignalClient.h
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(core PUBLIC
    Qt6::Network
    Qt6::Multimedia
)
