cmake_minimum_required(VERSION 3.12)

project(OSC)                                                       # set application name
set(UMFELD_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../umfeld") # set path to umfeld library e.g `set(UMFELD_PATH "<absolute/path/to/library>")`
link_directories("/usr/local/lib")                                 # optional, can help to fix issues with Homebrew on macOS

option(DISABLE_GRAPHICS "Disable graphic output" OFF)
option(DISABLE_VIDEO "Disable video output" ON)
option(DISABLE_AUDIO "Disable audio output + input" ON)

# set compiler flags to C++17 ( minimum required by umfeld, needs to go before `add_executable` )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "Build architectures for Mac OS X")

# add source + header files from this directory

file(GLOB SOURCE_FILES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
include_directories(".")

# add umfeld

add_subdirectory(${UMFELD_PATH} ${CMAKE_BINARY_DIR}/umfeld-lib-${PROJECT_NAME})
add_umfeld_libs()
