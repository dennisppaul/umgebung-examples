cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(PaulStretch VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include contrib)

# Add the source files
set(SOURCES
        src/ProcessedStretch.cpp
        contrib/kiss_fft.c
        contrib/kiss_fftr.c
)

# header files
set(HEADERS
        include/PaulStretch.h
        include/ProcessedStretch.h
        contrib/kiss_fft.h
        contrib/kiss_fftr.h
        contrib/_kiss_fft_guts.h
)

# create library
add_library(PaulStretch STATIC ${SOURCES} ${HEADERS})

# include directories for library
target_include_directories(PaulStretch PUBLIC ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/contrib)

# use KISSFFT
target_compile_definitions(PaulStretch PUBLIC KISSFFT)

# installation
# install(TARGETS PaulStretch DESTINATION lib)
# install(FILES ${HEADERS} DESTINATION include)

#
# usage:
#
# # add library directory
# add_subdirectory(path/to/library)
# 
# # create executable or another target
# add_executable(MyExecutable main.cpp)
# 
# # link library to executable
# target_link_libraries(MyExecutable PRIVATE PaulStretch)
#
