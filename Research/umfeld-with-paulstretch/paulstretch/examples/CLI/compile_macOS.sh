#! /bin/sh

outfile=paulstretch
rm -f $outfile

PAULSTRETCH_LIB_LOCATION="../.."

# Compile C source files with gcc
gcc -c \
-I./contrib \
../../contrib/*.c

# Compile C++ source files with g++
g++ -std=c++14 -DKISSFFT \
-c \
-I/opt/homebrew/include \
-I$PAULSTRETCH_LIB_LOCATION/contrib \
-I./input \
-I$PAULSTRETCH_LIB_LOCATION/include \
./input/*.cpp \
$PAULSTRETCH_LIB_LOCATION/src/*.cpp \
*.cpp

# Link all object files together with g++
g++ -std=c++14 \
*.o \
-L/opt/homebrew/lib -lz -lpthread -lmad \
-o $outfile

# Clean up object files if desired
rm -f *.o

#
# compile and run with `./compile_macOS.sh ; ./paulstretch INPUT.mp3 OUTPUT.raw`
# 