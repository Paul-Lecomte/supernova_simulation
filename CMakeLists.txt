cmake_minimum_required(VERSION 3.10)
project(supernova_simulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
file(GLOB SRC_FILES src/*.cpp main.cpp)

add_executable(supernova_simulation ${SRC_FILES})

# Find and link GLFW + OpenGL
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_include_directories(supernova_simulation PRIVATE include)
target_link_libraries(supernova_simulation PRIVATE OpenGL::GL glfw)