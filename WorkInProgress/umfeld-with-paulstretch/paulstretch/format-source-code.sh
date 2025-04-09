#! /bin/sh

echo "+++ formating the following files:"
echo 
find . -name '*.h' -o -name '*.c' -o -name '*.cpp'
echo

find . -name '*.h' -o -name '*.c' -o -name '*.cpp'  | xargs clang-format -i