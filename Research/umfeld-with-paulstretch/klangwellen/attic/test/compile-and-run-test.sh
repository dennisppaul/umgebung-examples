#! /bin/zsh

PROJECT=$1

if [ -n "$1" ]; then
    echo "+++ compiling ... : '$PROJECT.cpp'"
    if [ -n "$2" ]; then
        clang++ -std=c++17 $PROJECT.cpp -I../../include/ -I./ $2 -o $PROJECT
    else
        clang++ -std=c++17 $PROJECT.cpp -I../../include/ -I./ -o $PROJECT
    fi
    echo "+++ running ..... : '$PROJECT'"
    ./$PROJECT
else
    echo "+++ provide name of test to run e.g 'klangwellen' ( without '.cpp' extension )"
    exit
fi

