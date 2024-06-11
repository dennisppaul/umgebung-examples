cmake_minimum_required(VERSION 3.12)

project(umgebung-with-KLST_EMU)                                    # set application name
set(KLST_EMU_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libraries")     # set path to libraries

set(UMGEBUNG_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../umgebung") # set path to umgebung library e.g `set(UMGEBUNG_PATH "<absolute/path/to/library>")`
link_directories("/usr/local/lib")                                 # optional, can help to fix issues with Homebrew on macOS
link_directories("/opt/homebrew/lib/")

option(DISABLE_GRAPHICS "Disable graphic output" OFF)
option(DISABLE_VIDEO "Disable video output" ON)
option(DISABLE_AUDIO "Disable audio output + input" OFF)
option(ENABLE_PORTAUDIO "Enable Portaudio Driver for output + input" OFF)

# add source + header files from this directory

file(GLOB SOURCE_FILES 
    "*.cpp"
    "./klangstrom-emulator/src/*.cpp"
    "./klangstrom-libraries/Klangstrom/src/*.cpp"
    "./klangstrom-libraries/Klangstrom_KLST_EMU/src/*.cpp"
)
include_directories(
    "."
    "./klangstrom-emulator/src"
    "./klangstrom-libraries/Klangstrom/src"
    "./klangstrom-libraries/Klangstrom_KLST_EMU/src"
)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_compile_definitions(${PROJECT_NAME} PRIVATE UMGEBUNG_WINDOW_TITLE="${PROJECT_NAME}") # set window title
target_compile_definitions(${PROJECT_NAME} PUBLIC KLST_ENV=0x46)                            # set environment KLST_PANDA_EMU=0x46 KLST_CATERPILLAR_EMU=0x47

# add umgebung

add_subdirectory(${UMGEBUNG_PATH} ${CMAKE_BINARY_DIR}/umgebung-lib-${PROJECT_NAME})
add_umgebung_libs()

# set compiler flags to C++17 ( minimum required by umgebung )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGL_SILENCE_DEPRECATION")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++17-extensions")
set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "Build architectures for Mac OS X")
