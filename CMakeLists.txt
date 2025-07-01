cmake_minimum_required(VERSION 3.31)
project(Engine)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(vendor/glfw)
add_subdirectory(vendor/assimp)
add_subdirectory(vendor/entt)

include_directories(
    vendor/include 
    vendor/assimp/include 
    vendor/glfw/include 
    vendor/entt/single_include
    vendor/imgui
)

add_executable(Engine
    src/glad.c
    src/main.cpp
    src/shader.cpp
    src/camera.cpp
    src/mesh.cpp
    src/model.cpp
    src/scene.cpp
    src/editor.cpp
    src/window.cpp

    vendor/imgui/imconfig.h
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui.h
    vendor/imgui/imgui_demo.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_internal.h
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/imstb_rectpack.h
    vendor/imgui/imstb_textedit.h
    vendor/imgui/imstb_truetype.h

    vendor/imgui/backends/imgui_impl_glfw.cpp
    vendor/imgui/backends/imgui_impl_glfw.h
    vendor/imgui/backends/imgui_impl_opengl3.cpp
    vendor/imgui/backends/imgui_impl_opengl3.h

    vendor/imgui/misc/cpp/imgui_stdlib.cpp
    vendor/imgui/misc/cpp/imgui_stdlib.h
 "src/input.cpp")

target_link_libraries(Engine PRIVATE glfw assimp)