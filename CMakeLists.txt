cmake_minimum_required(VERSION 3.16)

set (CMAKE_CXX_STANDARD 17)

project(Barycentric)
configure_file(${PROJECT_SOURCE_DIR}/configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)
include_directories( ${PROJECT_SOURCE_DIR}/includes )
link_directories("${PROJECT_SOURCE_DIR}/.libs")

add_executable(Barycentric stb_image/stb_image.cpp Shader.h glad.c Barycentric.cpp)

target_link_libraries(Barycentric glfw3)
