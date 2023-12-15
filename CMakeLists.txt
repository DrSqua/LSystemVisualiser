cmake_minimum_required(VERSION 3.26)
project(LSystemVisualiser)

set(CMAKE_CXX_STANDARD 17)

add_executable(LSystemVisualiser main.cpp
        LSystemInterpreter.hpp
        LSystemInterpreter.cpp)
