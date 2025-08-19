cmake_minimum_required(VERSION 3.10)
project(supernova_simulation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

add_executable(supernova_simulation main.cpp)

target_link_libraries(supernova_simulation PRIVATE OpenGL::GL)