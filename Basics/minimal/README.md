to compile and run this example just type the following commands in the terminal:

```sh
$ cmake -B build      # prepare build system
$ cmake --build build # compile the application
$ ./build/minimal     # to run the application
```

there is also a way to concatenate all three commands in one line like this:

```sh
$ cmake -B build ; cmake --build build ; ./build/minimal
```

to start a *clean* build simply delete the build directory:

```sh
$ rm -rf build
```

## template for a CMake file

this minimal cmake example shows how to configure CMake to build an umfeld application. the only *really* important thing is to set the `UMFELD_PATH` variable to the location of the umfeld repository ( i.e the folder containing `LICENSE.md`, `README.md`, `src` etcetera ) e.g `$HOME/Documents/dev/umfeld/git/umfeld`. all other lines should be fine as they are:

```cmake
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
```

## BONUS: custom run target

as a bonus, the following command can be appended to the CMake script to add a run target. this allows to build and run in a single command like by adding `--target run` i.e `cmake --build build --target run`  

```cmake
add_custom_target(run  
        COMMAND ${PROJECT_NAME}  
        DEPENDS ${PROJECT_NAME}  
        WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}  
)
```
