
#pragma once

#include <glm/glm.hpp>

#include "shaderProgram.hpp"
#include "camera.hpp"

class LightCube
{

private:
    unsigned int VAO, VBO;

    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;

    ShaderProgram* m_program;

public:
    LightCube();
    ~LightCube();

    void draw(Camera* camera);
    void load();
    void destroy();

    void setPosition(glm::vec3 pos);
    void setRotation(glm::vec3 rotation);
    void setScale(glm::vec3 scale);
    void setColor(glm::vec3 rgb_color);

    void move(glm::vec3 value);
    void rotate(glm::vec3 value);
    void scale(glm::vec3 value);

    glm::vec3 getColor();
    glm::vec3 getPosition();

    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
};