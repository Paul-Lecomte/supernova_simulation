cmake_minimum_required(VERSION 3.10)
project(supernova_simulation)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fichiers sources
file(GLOB SRC_FILES
        src/*.cpp
        main.cpp
)

# Ajout de SOIL (statique)
add_library(SOIL STATIC
        external/soil/src/soil.c
        external/soil/src/image_dxt.c
        external/soil/src/image_helper.c
        external/soil/src/stb_image_aug.c
)
target_include_directories(SOIL PUBLIC external/soil/include)

# Ajout de GLFW (vendored)
add_subdirectory(external/glfw)

# Exécutable principal
add_executable(supernova_simulation ${SRC_FILES})

# Includes
target_include_directories(supernova_simulation PRIVATE
        include
        external/glm
        external/soil/include
)

# Lien des librairies
target_link_libraries(supernova_simulation PRIVATE
        glfw
        SOIL
)

# OpenGL et GLU
find_package(OpenGL REQUIRED)
target_link_libraries(supernova_simulation PRIVATE OpenGL::GL glu32)

# Windows : forcer opengl32 si besoin
if(WIN32)
    target_link_libraries(supernova_simulation PRIVATE opengl32)
endif()