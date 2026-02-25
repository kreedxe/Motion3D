
#pragma once

#include <glm/glm.hpp>

#include "shaderProgram.hpp"
#include "camera.hpp"
#include "light.hpp"

class Cube
{

private:
    unsigned int VAO, VBO;
    unsigned int m_texture;

    glm::vec3 m_rotation;
    glm::vec3 m_scale;

    ShaderProgram* m_program;
    Camera* m_camera;
    LightCube* m_light;

    
public:
    Cube();
    ~Cube();
    
    void draw();
    void load(const char* texturePath, Camera* camera, LightCube* light);
    void destroy();
    
    void setPosition(glm::vec3 pos);
    void setRotation(glm::vec3 rotation);
    void setScale(glm::vec3 scale);
    
    void move(glm::vec3 value);
    void rotate(glm::vec3 value);
    void scale(glm::vec3 value);
    
    float m_shininess;
    glm::vec3 m_position;
};