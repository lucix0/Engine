cmake_minimum_required(VERSION 3.31)
project(Engine)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(glfw)
add_subdirectory(assimp)

include_directories(include assimp/include)

add_executable(Engine
        src/glad.c
        src/main.cpp
        src/shader.cpp
 "src/camera.cpp")

target_link_libraries(Engine PRIVATE glfw assimp)