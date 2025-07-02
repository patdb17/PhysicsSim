cmake_minimum_required(VERSION 3.20)

set(CMAKE_VERBOSE_MAKEFILE ON)
project(PhysicsSim VERSION 0.1.0 LANGUAGES C CXX)

# This is so that VSCode can use the compile_commands.json file for IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# add_executable(PhysicsSim src/Main.cpp)
file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(PhysicsSim ${SOURCES})

# Find OpenGL
find_package(OpenGL REQUIRED)

# Fetch GLFW
set(FETCHCONTENT_QUIET OFF)
include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

# Fetch GLEW build and link statically
include(FetchContent)
FetchContent_Declare(
    glew
    URL https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0.tgz
)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE) # Force static build
FetchContent_MakeAvailable(glew)

# Configure GLEW to build statically and disable utilities
set(BUILD_UTILS OFF CACHE BOOL "Build utilities" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)

# Add GLEW's CMake build system
add_subdirectory(${glew_SOURCE_DIR}/build/cmake ${glew_BINARY_DIR})

# Link external libraries (glfw and glew)
target_link_libraries(PhysicsSim PRIVATE OpenGL::GL glfw glew_s)

target_include_directories(PhysicsSim PRIVATE ${glew_SOURCE_DIR}/include src/)

# Set C++ standard
set_property(TARGET PhysicsSim PROPERTY CXX_STANDARD 23)