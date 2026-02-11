cmake_minimum_required(VERSION 3.28)
project(amogus)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#------------------------------------------------
# FIND REQUIRED LIBRARIES
#------------------------------------------------
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)


#------------------------------------------------
# BUILD GLAD STATIC LIBRARY
#------------------------------------------------
add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)


#------------------------------------------------
# BUILD IMGUI STATIC LIBRARY
#------------------------------------------------
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


#------------------------------------------------
# BUILD MAIN EXECUTABLE
#------------------------------------------------
set(SOURCES 
    src/main.cpp 
    src/shaders.cpp 
    src/camera.cpp
    src/cube.cpp)

add_executable(amongUs ${SOURCES})
target_link_libraries(amongUs PRIVATE glfw glad OpenGL::GL imgui glm::glm)
target_include_directories(amongUs PRIVATE external/stb)