cmake_minimum_required(VERSION 3.12)

project(game-controller)                                           # set application name
set(UMGEBUNG_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../umgebung") # set path to umgebung library e.g `set(UMGEBUNG_PATH "<absolute/path/to/library>")`
link_directories("/usr/local/lib")                                 # optional, can help to fix issues with Homebrew on macOS
link_directories("/opt/homebrew/lib/")

# set compiler flags to C++17 ( minimum required by umgebung, needs to go before `add_executable` )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "Build architectures for Mac OS X")

# add source + header files from this directory

file(GLOB SOURCE_FILES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
include_directories(".")

# add umgebung

target_compile_definitions(${PROJECT_NAME} PRIVATE UMGEBUNG_WINDOW_TITLE="${PROJECT_NAME}") # set window title
add_subdirectory(${UMGEBUNG_PATH} ${CMAKE_BINARY_DIR}/umgebung-lib-${PROJECT_NAME})
add_umgebung_libs()
