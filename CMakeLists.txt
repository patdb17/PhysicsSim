cmake_minimum_required(VERSION 3.20)

set(CMAKE_VERBOSE_MAKEFILE ON)
project(PhysicsSim VERSION 0.1.0 LANGUAGES C CXX)

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

# Fetch GLEW
# set(GLEW_USE_STATIC_LIBS TRUE)
# include(FetchContent)
# FetchContent_Declare(
#     glew
#     GIT_REPOSITORY https://github.com/nigels-com/glew.git
#     GIT_TAG glew-2.2.0
#     #SOURCE_SUBDIR build/cmake
# )
# FetchContent_MakeAvailable(glew)
find_package(GLEW REQUIRED)

# add_executable(PhysicsSim src/Main.cpp)
file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(PhysicsSim ${SOURCES})

# Link external libraries (glfw and glew)
target_link_libraries(PhysicsSim PRIVATE OpenGL::GL glfw GLEW::GLEW)

target_include_directories(PhysicsSim PRIVATE src/)# ${glew_SOURCE_DIR}/include)

# Set C++ standard
set_property(TARGET PhysicsSim PROPERTY CXX_STANDARD 23)