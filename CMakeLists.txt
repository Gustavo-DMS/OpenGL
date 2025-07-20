cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

project(Engine VERSION 1.0)


file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
add_executable(Engine ${MY_SOURCES} main.c)
target_include_directories(Engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

add_subdirectory("libs/glfw-3.4")
add_subdirectory("libs/glad")

target_link_libraries(Engine PRIVATE glfw glad)
