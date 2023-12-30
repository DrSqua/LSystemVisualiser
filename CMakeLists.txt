cmake_minimum_required(VERSION 3.26)
project(LSystemVisualiser)

set(CMAKE_CXX_STANDARD 17)

# Library ----- ----- -----
#   Create Library
#
add_library(LSystemLib STATIC
        lsystemsource/Production.cpp
)

#   Define header files for Lib
#
target_include_directories(LSystemLib PRIVATE "include/")
# End Library ----- ----- -----



# Test suite ----- ----- -----
#   Create executable for test
add_executable(TestSuite test/main.cpp
                        test/test_lsystem.cpp)

# Similar to what we did earlier, we tell CMake where "TestSuite" is supposed to find our headers
target_include_directories(TestSuite PRIVATE "include/")

# "LSystemLib" should be a part of "TestSuite"
target_link_libraries(TestSuite PRIVATE LSystemLib)
# End Test suite ----- ----- -----



# Raylib ----- ----- -----
# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
set(RAYLIB_VERSION 5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
            raylib
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()
# End Raylib ----- ----- -----

# Executable visualizer
add_executable(${PROJECT_NAME} src/main.cpp
        src/LSystemDrawing.cpp
        src/LSystemDrawing.hpp)
#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib LSystemLib)

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif()
