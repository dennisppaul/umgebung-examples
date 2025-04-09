cmake_minimum_required(VERSION 3.12)

project(klangwellen)

add_library(klangwellen INTERFACE)

target_include_directories(klangwellen INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
