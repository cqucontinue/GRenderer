cmake_minimum_required(VERSION 3.10)
project(gRender)

set(CMAKE_CXX_STANDARD 14)

add_executable(gRender main.cpp Matrix.h Object.h Render.h Painter.h Color.h Shader.h)
target_link_libraries(gRender gdi32 user32)
set(EXECUTABLE_OUTPUT_PATH e:/project/${PROJECT_NAME})