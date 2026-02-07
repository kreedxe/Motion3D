cmake_minimum_required(VERSION 4.2)
project(amogus)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# find required libraries
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)


# create GLAD static library
add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)


# create imgui static library
add_library(imgui STATIC
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_demo.cpp

    external/imgui/backend/imgui_impl_glfw.cpp
    external/imgui/backend/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC external/imgui)


# source files
set(SOURCES src/main.cpp src/shaders.cpp)


# main executable
add_executable(amongUs ${SOURCES})


# link libraries
target_link_libraries(amongUs PRIVATE glfw glad OpenGL::GL imgui glm::glm)
