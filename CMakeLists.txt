cmake_minimum_required(VERSION 3.24)
project (InsaneProceduralTerrain)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(CMAKE_CXX_STANDARD 20)

set(EXECUTABLE_TARGET_NAME opengl-insane-procedural-terrain)

find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

file(GLOB_RECURSE src_files "source/*.h")

# Put all CPP files here.
add_executable(${EXECUTABLE_TARGET_NAME}
    "source/main.cpp"
    "source/Rendering/Shader.cpp"
    "source/SceneObjects/Camera/Camera.cpp"
    "source/ProceduralGeneration/HeightMap/HeightMap.cpp" "source/Noise/Noise.h")

# All Resources/ files are copied into out/ after building program
add_custom_command(TARGET ${EXECUTABLE_TARGET_NAME} PRE_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_SOURCE_DIR}/source/Resources/ $<TARGET_FILE_DIR:${EXECUTABLE_TARGET_NAME}>/Resources)

target_link_libraries(${EXECUTABLE_TARGET_NAME} PRIVATE glfw)
target_link_libraries(${EXECUTABLE_TARGET_NAME} PRIVATE GLEW::GLEW)
target_link_libraries(${EXECUTABLE_TARGET_NAME} PRIVATE imgui::imgui)
target_link_libraries(${EXECUTABLE_TARGET_NAME} PRIVATE glm::glm)