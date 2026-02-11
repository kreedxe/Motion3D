
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera
{

private:
    glm::vec3  pos         =  glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3  front       =  glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3  up          =  glm::vec3(0.0f, 1.0f, 0.0f);

    float      yaw         = -90.0f;
    float      pitch       =  0.0f;
    float      lastX       =  800.0f / 2.0;
    float      lastY       =  600.0 / 2.0;
    float      fov         =  45.0f;
    float      sensivity   =  0.1f;
    
public:
    bool firstMouse = true;
    
    Camera();
    ~Camera();

    void mouseCallback(double xpos, double ypos);
    void scrollCallback(double xoffset, double yoffset);
    void keyboardCallback(GLFWwindow* window, float deltaTime);
    
    glm::mat4 getView();
    glm::mat4 getProjection();

};