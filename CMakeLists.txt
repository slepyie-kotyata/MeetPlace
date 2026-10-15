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

# include(FetchContent)

# FetchContent_Declare(
#     libdatachannel
#     GIT_REPOSITORY https://github.com/paullouisageneau/libdatachannel.git
#     GIT_TAG 6b1e2e620f1e37f0eafeee702eaea0043cb305fd
# )

# FetchContent_MakeAvailable(libdatachannel)

qt_standard_project_setup(REQUIRES 6.10)

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/importedcontent/CMakeLists.txt")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/importedcontent)
endif()

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/tests/media-sandbox")
    add_subdirectory(tests/media-sandbox)
endif()

add_subdirectory(core)
add_subdirectory(app)
