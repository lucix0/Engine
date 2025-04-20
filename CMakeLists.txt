cmake_minimum_required(VERSION 3.31)
project(Engine)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(glfw)
add_subdirectory(bgfx.cmake)
add_subdirectory(assimp)

include_directories(include)

add_executable(Engine
        src/main.cpp
        src/util.cpp
        src/mesh.cpp
        include/util.hpp
        include/mesh.hpp
)

target_link_libraries(Engine PRIVATE glfw bgfx bimg bx)

target_compile_definitions(Engine PRIVATE BX_CONFIG_DEBUG=0)
