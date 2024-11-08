## --- Project configuration --- ##
cmake_minimum_required(VERSION 3.20)
project(Calculator)

## --- Compiler configuration --- ##
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)


## --- Executable configuration --- ##
add_executable(Calculator "source/main.cpp")
