cmake_minimum_required(VERSION 3.31)
project(Engine)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(vendor/glfw)
add_subdirectory(vendor/assimp)
add_subdirectory(vendor/entt)

include_directories(vendor/include vendor/assimp/include vendor/entt/single_include)

add_executable(Engine
        src/glad.c
        src/main.cpp
        src/shader.cpp
 "src/camera.cpp" "src/mesh.cpp" "src/model.cpp" "src/scene.hpp" "src/scene.cpp" "src/editor.cpp" "src/window.cpp")

target_link_libraries(Engine PRIVATE glfw assimp)