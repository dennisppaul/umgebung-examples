cmake_minimum_required(VERSION 3.12)

project(umgebung-with-KLST_EMU)                                    # set application name
set(LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libraries")          # set path to libraries

set(UMGEBUNG_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../umgebung") # set path to umgebung library e.g `set(UMGEBUNG_PATH "<absolute/path/to/library>")`
link_directories("/usr/local/lib")                                 # optional, can help to fix issues with Homebrew on macOS
link_directories("/opt/homebrew/lib/")

set(KLST_ENV 0x46)                                                 # set KLST environment 
                                                                   # 0x46=KLST_PANDA_EMU
                                                                   # 0x47=KLST_CATERPILLAR_EMU

# set compiler flags to C++17 ( minimum required by umgebung )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGL_SILENCE_DEPRECATION")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++17-extensions")
set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "Build architectures for Mac OS X")

# add source + header files from this directory

file(GLOB SOURCE_FILES 
    "*.cpp"
)
include_directories(
    "."
)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_compile_definitions(${PROJECT_NAME} PUBLIC KLST_ENV=${KLST_ENV})

# add umgebung

option(DISABLE_GRAPHICS "Disable graphic output" OFF)
option(DISABLE_VIDEO "Disable video output" ON)
option(DISABLE_AUDIO "Disable audio output + input" OFF)
option(ENABLE_PORTAUDIO "Enable Portaudio Driver for output + input" OFF)

target_compile_definitions(${PROJECT_NAME} PRIVATE UMGEBUNG_WINDOW_TITLE="${PROJECT_NAME}") # set window title

add_subdirectory(${UMGEBUNG_PATH} ${CMAKE_BINARY_DIR}/umgebung-lib-${PROJECT_NAME})
add_umgebung_libs()

# add other libraries

add_subdirectory(${LIBRARY_PATH}/klangstrom-emulator ${CMAKE_BINARY_DIR}/klangstrom-emulator-lib-${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PUBLIC klangstrom-emulator)

add_subdirectory(${LIBRARY_PATH}/klangstrom-libraries/Klangstrom ${CMAKE_BINARY_DIR}/klangstrom-lib-${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PUBLIC klangstrom)

add_subdirectory(${LIBRARY_PATH}/klangstrom-libraries/Klangstrom_KLST_EMU ${CMAKE_BINARY_DIR}/klangstrom_KLST_EMU-lib-${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PUBLIC klangstrom_KLST_EMU)

add_subdirectory(${LIBRARY_PATH}/klangwellen ${CMAKE_BINARY_DIR}/klangwellen-lib-${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PUBLIC klangwellen)
