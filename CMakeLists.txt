cmake_minimum_required(VERSION 3.16)

project(basic LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(assimp REQUIRED)

add_library(glad STATIC
  thirdparty/glad/src/glad.c
)
target_include_directories(glad PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad/include
)

add_library(stb_image STATIC
  thirdparty/stb/stb_image.c
)
target_include_directories(stb_image PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/stb
)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glm
)

add_executable(basic
  src/main.cpp
  graphics/Mesh/Mesh.cpp
  graphics/Shader/Shader.cpp
  graphics/Camera/Camera.cpp
  graphics/Model/Model.cpp
)

target_include_directories(basic PRIVATE
  graphics
)

target_link_libraries(basic PRIVATE
  glfw
  OpenGL::GL
  assimp::assimp
  glad
  stb_image
  glm
)

add_custom_command(TARGET basic POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:basic>/resources"
    COMMENT "Copying resources/ to build output"
)
