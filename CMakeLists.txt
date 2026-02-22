cmake_minimum_required(VERSION 3.28)
project(amogus)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)


#------------------------------------------------
# FIND OPENGL
#------------------------------------------------
find_package(OpenGL REQUIRED)


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
# BUILD STB IMAGE STATIC LIBRARY
#------------------------------------------------
add_library(stb_image STATIC external/stb/stb_image_impl.c)
target_include_directories(stb_image PUBLIC external/stb)


#------------------------------------------------
# ADD GLM LIBRARY
#------------------------------------------------
add_subdirectory(external/glm)


#------------------------------------------------
# ADD GLFW LIBRARY
#------------------------------------------------
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)


#------------------------------------------------
# BUILD MAIN EXECUTABLE
#------------------------------------------------
set(SOURCES 
    src/main.cpp 
    src/shaderProgram.cpp 
    src/camera.cpp
    src/cube.cpp
    src/light.cpp
)

add_executable(amongUs ${SOURCES})
target_link_libraries(amongUs PRIVATE glfw glad OpenGL::GL imgui glm::glm stb_image)
