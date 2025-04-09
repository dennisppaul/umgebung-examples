# minimal umfeld example
#
# this minimal cmake example shows how to configure CMake to build a umfeld application.
# the only really important thing is to set the `UMFELD_PATH` variable to the location
# of the umfeld repository ( i.e the folder containing `LICENSE.md`, `README.md`, `src` etcetera ).
# all other lines should be fine as they are.

cmake_minimum_required(VERSION 3.12)

project(minimal)                                                   # set application name
set(UMFELD_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../umfeld") # set path to umfeld library

# --------- no need to change anything below this line ------------

set(CMAKE_CXX_STANDARD 17)                                         # set c++ standard, this needs to happen before `add_executable`
set(CMAKE_CXX_STANDARD_REQUIRED ON)                                # minimum is C++17 but 20 and 23 should also be fine

include_directories(".")                                           # add all `.h` header files from this directory
file(GLOB SOURCE_FILES "*.cpp")                                    # collect all `.cpp` source files from this directory
add_executable(${PROJECT_NAME} ${SOURCE_FILES})                    # add source files to application

add_subdirectory(${UMFELD_PATH} ${CMAKE_BINARY_DIR}/umfeld-lib-${PROJECT_NAME}) # add umfeld location
add_umfeld_libs()                                                # add umfeld library
