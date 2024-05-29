cmake_minimum_required(VERSION 3.10)

project(PaulStretchProject VERSION 1.0)
add_executable(PaulStretchTest cmake-example.cpp)

# add library
add_subdirectory(../../ ${CMAKE_BINARY_DIR}/PaulStretch)

# link library
target_link_libraries(PaulStretchTest PRIVATE PaulStretch)

#
# build + run with `cmake -B build . ; cmake --build build ; ./build/PaulStretchTest`
#