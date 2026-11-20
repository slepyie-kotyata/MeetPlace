cmake_minimum_required(VERSION 3.22)

project(MeetPlace
    VERSION 0.1.0
    LANGUAGES C CXX
)

# Project configuration

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Qt

find_package(Qt6 6.10 REQUIRED COMPONENTS
    Core
    Quick
    VirtualKeyboard
    Multimedia
    Network
    WebSockets
)

qt_standard_project_setup(REQUIRES 6.10)

# Dependencies

include(FetchContent)

# Mbed TLS

set(ENABLE_PROGRAMS OFF CACHE BOOL "" FORCE)
set(ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(MBEDTLS_FATAL_WARNINGS OFF CACHE BOOL "" FORCE)

set(USE_STATIC_MBEDTLS_LIBRARY ON CACHE BOOL "" FORCE)
set(USE_SHARED_MBEDTLS_LIBRARY OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    mbedtls
    GIT_REPOSITORY https://github.com/Mbed-TLS/mbedtls.git
    GIT_TAG mbedtls-3.6.7
    GIT_SUBMODULES_RECURSE TRUE
)

FetchContent_MakeAvailable(mbedtls)

list(PREPEND CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
)

# libdatachannel

set(USE_MBEDTLS ON CACHE BOOL "" FORCE)
set(USE_GNUTLS OFF CACHE BOOL "" FORCE)

set(ENABLE_MBEDTLS OFF CACHE BOOL "" FORCE)
set(ENABLE_OPENSSL OFF CACHE BOOL "" FORCE)
set(ENABLE_NSS OFF CACHE BOOL "" FORCE)

set(PREFER_SYSTEM_LIB OFF CACHE BOOL "" FORCE)
set(USE_SYSTEM_SRTP OFF CACHE BOOL "" FORCE)
set(USE_SYSTEM_JUICE OFF CACHE BOOL "" FORCE)
set(USE_SYSTEM_USRSCTP OFF CACHE BOOL "" FORCE)
set(USE_SYSTEM_PLOG OFF CACHE BOOL "" FORCE)
set(USE_SYSTEM_JSON OFF CACHE BOOL "" FORCE)

set(NO_EXAMPLES ON CACHE BOOL "" FORCE)
set(NO_TESTS ON CACHE BOOL "" FORCE)
set(LIBSRTP_TEST_APPS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    libdatachannel
    GIT_REPOSITORY https://github.com/paullouisageneau/libdatachannel.git
    GIT_TAG 6b1e2e620f1e37f0eafeee702eaea0043cb305fd
    GIT_SUBMODULES_RECURSE TRUE
)

FetchContent_MakeAvailable(libdatachannel)

# Project modules

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/importedcontent/CMakeLists.txt")
    add_subdirectory(importedcontent)
endif()

add_subdirectory(core)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/media-sandbox/CMakeLists.txt")
    add_subdirectory(tests/media-sandbox)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/call-sandbox/CMakeLists.txt")
    add_subdirectory(tests/call-sandbox)
endif()

add_subdirectory(app)
