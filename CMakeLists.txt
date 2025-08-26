cmake_minimum_required(VERSION 3.10)
project(supernova_simulation)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
file(GLOB SRC_FILES src/*.cpp main.cpp)

# Build executable
add_executable(supernova_simulation ${SRC_FILES})

# Include headers
target_include_directories(supernova_simulation PRIVATE
        include
        external/glm   # Add GLM headers here
)

# Add GLFW (vendored)
add_subdirectory(external/glfw)
target_link_libraries(supernova_simulation PRIVATE glfw)

# Find and link OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(supernova_simulation PRIVATE OpenGL::GL)