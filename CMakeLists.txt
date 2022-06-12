cmake_minimum_required(VERSION 3.10)

# set the project name
project(main)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/..")

# add the executable
#add_executable(opengl_tut main.cpp)
add_executable(main main.cpp shader.cpp)

target_link_libraries(main GL GLEW glfw)

