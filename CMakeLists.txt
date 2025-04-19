cmake_minimum_required(VERSION 3.31)
project(Engine)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(bgfx.cmake)

# Set glfw3 include and library paths
include_directories(/opt/homebrew/Cellar/glfw/3.4/include)
link_directories(/opt/homebrew/Cellar/glfw/3.4/lib)

# Find the glfw3 package
find_package(glfw3 3.4 REQUIRED)

add_executable(Engine main.cpp)

# Link glfw3 library
target_link_libraries(Engine PRIVATE glfw bgfx bimg bx)

target_compile_definitions(Engine PRIVATE BX_CONFIG_DEBUG=0)
