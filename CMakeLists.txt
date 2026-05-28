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
        src/camera.cpp
        src/mesh.cpp
        src/model.cpp
        src/window.cpp)

bgfx_compile_shaders(
        TYPE VERTEX
        SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/src/ObjectVertex.sc
        VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/src/varying.def.sc
        OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/shaders/
)

bgfx_compile_shaders(
        TYPE FRAGMENT
        SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/src/ObjectFragment.sc
        VARYING_DEF ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/src/varying.def.sc
        OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/shaders/
)

target_link_libraries(Engine PRIVATE bgfx glfw assimp)