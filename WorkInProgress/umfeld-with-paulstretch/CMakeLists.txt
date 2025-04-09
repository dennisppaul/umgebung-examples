cmake_minimum_required(VERSION 3.20)

project(paulstretch-app)                                           # set application name
set(UMFELD_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../umfeld") # set path to umfeld library

# --------- no need to change anything below this line ------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(".")
file(GLOB SOURCE_FILES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_subdirectory(${UMFELD_PATH} ${CMAKE_BINARY_DIR}/umfeld-lib-${PROJECT_NAME})
add_umfeld_libs()

# add klangwellen library

include_directories("./klangwellen/include")

# add PaulStretch

add_subdirectory(paulstretch ${CMAKE_BINARY_DIR}/PaulStretch)
target_link_libraries(${PROJECT_NAME} PRIVATE PaulStretch)
