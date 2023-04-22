cmake_minimum_required(VERSION 3.24)
project (InsaneProceduralTerrain)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(CMAKE_CXX_STANDARD 20)

set(EXECUTABLE_TARGET_NAME opengl-insane-procedural-terrain)

file(GLOB_RECURSE src_files
        "source/*.h"
        "source/*.cpp"
        "source/*.frag"
        "source/*.vert")

find_package(SFML COMPONENTS system window graphics network CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(GLEW REQUIRED)

add_executable(${EXECUTABLE_TARGET_NAME}
    "source/main.cpp" "source/Shaders/Shader.cpp")

target_link_libraries(${EXECUTABLE_TARGET_NAME} PRIVATE sfml-system sfml-network sfml-graphics sfml-window)
target_link_libraries(${EXECUTABLE_TARGET_NAME} PRIVATE GLEW::GLEW)
target_link_libraries(${EXECUTABLE_TARGET_NAME} PRIVATE imgui::imgui)