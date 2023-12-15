cmake_minimum_required(VERSION 3.26)
project(LSystemVisualiser)

set(CMAKE_CXX_STANDARD 17)

#   Create Library
#
add_library(LSystemLib STATIC
        src/Production.cpp
)

#   Define header files for Lib
#
target_include_directories(LSystemLib PRIVATE "include/")


#   Create executable for test
add_executable(TestSuite test/main.cpp
                        test/test_lsystem.cpp)

# Similar to what we did earlier, we tell CMake where "TestSuite" is supposed to find our headers
target_include_directories(TestSuite PRIVATE "include/")

# "ListLib" should be a part of "TestSuite"
target_link_libraries(TestSuite PRIVATE LSystemLib)