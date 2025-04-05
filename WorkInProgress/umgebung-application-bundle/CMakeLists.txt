cmake_minimum_required(VERSION 3.12)

project(umgebung-application-bundle)                               # set application name
set(UMGEBUNG_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../umgebung") # set path to umgebung library e.g `set(UMGEBUNG_PATH "<absolute/path/to/library>")`
link_directories("/usr/local/lib")                                 # optional, can help to fix issues with Homebrew on macOS
link_directories("/opt/homebrew/lib/")

option(DISABLE_GRAPHICS "Disable graphic output" OFF)
option(DISABLE_VIDEO "Disable video output" ON)
option(DISABLE_AUDIO "Disable audio output + input" OFF)

# set compiler flags to C++17 ( minimum required by umgebung, needs to go before `add_executable` )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "Build architectures for Mac OS X")

# add source + header files from this directory

file(GLOB SOURCE_FILES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
include_directories(".")

target_compile_definitions(${PROJECT_NAME} PRIVATE UMGEBUNG_WINDOW_TITLE="${PROJECT_NAME}") # set window title

# add umgebung

add_subdirectory(${UMGEBUNG_PATH} ${CMAKE_BINARY_DIR}/umgebung-lib-${PROJECT_NAME})
add_umgebung_libs()

# make application bundle

# bundle assets
# install(FILES "${PROJECT_SOURCE_DIR}/RobotoMono-Regular.ttf" DESTINATION "../")   # include single resources files or ...
install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets/ DESTINATION "../assets")              # ... entire directories

# bundle dynamic libraries
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Bundling dynamic libraries using dylibbundler..."
    COMMAND dylibbundler -od -b -x $<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME} -d $<TARGET_FILE_DIR:${PROJECT_NAME}>/../libs/
    COMMENT ${CMAKE_COMMAND} -E echo "Dynamic libraries are bundled."
)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/libs/ DESTINATION "../libs")

# configure cpack
set(CPACK_PACKAGE_NAME "Umgebung")
set(CPACK_PACKAGE_VENDOR "Dennis P Paul")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Umgebung is an Environment")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")

set(CPACK_GENERATOR "Bundle")
set(CPACK_BUNDLE_NAME Umgebung)
set(CPACK_BUNDLE_ICON ${CMAKE_SOURCE_DIR}/Resources/Umgebung.icns)
set(CPACK_BUNDLE_PLIST ${CMAKE_SOURCE_DIR}/Resources/Info.plist)

set(CPACK_BUNDLE_STARTUP_COMMAND ${PROJECT_NAME})
include(CPack)

# run with `cmake -B build ; cmake --build build ; cd build ; cpack ; cd ..` from sketch folder
