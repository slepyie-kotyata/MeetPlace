cmake_minimum_required(VERSION 3.16)

project(MeetPlace VERSION 0.1 LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS 
    Core   
    Quick 
    VirtualKeyboard 
    Multimedia 
    Network 
    WebSockets
)
find_package(LibDataChannel REQUIRED)

qt_standard_project_setup(REQUIRES 6.10)

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/importedcontent/CMakeLists.txt")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/importedcontent)
endif()

add_subdirectory(core)

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/tests/media-sandbox")
    add_subdirectory(tests/media-sandbox)
endif()

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/tests/call-sandbox")
    add_subdirectory(tests/call-sandbox)
endif()

add_subdirectory(app)
