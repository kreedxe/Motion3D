
#include "camera.hpp"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::mouseCallback(double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
  
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensivity;
    yoffset *= sensivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);
}


void Camera::scrollCallback(double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}


void Camera::keyboardCallback(GLFWwindow* window, float deltaTime)
{
    const float cameraSpeed = 0.05f * deltaTime;
        
    if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS))
        pos += cameraSpeed * front;
    if ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS))
        pos -= cameraSpeed * front;
    if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS))
        pos -= glm::normalize(glm::cross(front, up)) * cameraSpeed;
    if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS))
        pos += glm::normalize(glm::cross(front, up)) * cameraSpeed;
}


glm::mat4 Camera::getView()
{
    glm::mat4 view;
    view = glm::lookAt(pos, pos + front, up);

    return view;
}


glm::mat4 Camera::getProjection()
{
    return glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
}
