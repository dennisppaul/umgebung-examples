cmake_minimum_required(VERSION 3.12)

project(umfeld-simple)                                           # set application name
set(UMFELD_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../umfeld") # set path to umfeld library e.g `set(UMFELD_PATH "<absolute/path/to/library>")`
link_directories("/usr/local/lib")                                 # optional, can help to fix issues with Homebrew on macOS
link_directories("/opt/homebrew/lib/")

option(DISABLE_GRAPHICS "Disable graphic output" OFF)
option(DISABLE_VIDEO "Disable video output" OFF)
option(DISABLE_AUDIO "Disable audio output + input" OFF)
option(ENABLE_PORTAUDIO "Enable Portaudio Driver for output + input" OFF)
option(DISABLE_MAIN "Disable main function entry point" OFF)

# set compiler flags to C++17 ( minimum required by umfeld, needs to go before `add_executable` )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "Build architectures for Mac OS X")

# add source + header files from this directory

file(GLOB SOURCE_FILES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
include_directories(".")

# add umfeld
target_compile_definitions(${PROJECT_NAME} PRIVATE UMFELD_WINDOW_TITLE="${PROJECT_NAME}") # set window title
add_subdirectory(${UMFELD_PATH} ${CMAKE_BINARY_DIR}/umfeld-lib-${PROJECT_NAME})
add_umfeld_libs()
