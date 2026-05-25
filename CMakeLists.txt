cmake_minimum_required(VERSION 3.31)
project(Engine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

set(BGFX_BUILD_EXAMPLES OFF CACHE BOOL "Disable BGFX examples" FORCE)

FetchContent_Declare(
        bgfx-cmake
        GIT_REPOSITORY https://github.com/bkaradzic/bgfx.cmake.git
        GIT_TAG 55fbf53
)
FetchContent_MakeAvailable(bgfx-cmake)

add_subdirectory(vendor/glfw)
add_subdirectory(vendor/assimp)
add_subdirectory(vendor/entt)

include_directories(vendor/include vendor/assimp/include vendor/entt/single_include)

add_executable(Engine
        src/glad.c
        src/main.cpp
        src/shader.cpp
 "src/camera.cpp" "src/mesh.cpp" "src/model.cpp" "src/scene.hpp"  "src/window.cpp")

target_link_libraries(Engine PRIVATE bgfx glfw assimp)