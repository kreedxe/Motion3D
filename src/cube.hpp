
#pragma once

#include <glm/glm.hpp>

#include "shaders.hpp"
#include "camera.hpp"

class Cube
{

private:
    unsigned int VAO, VBO, texture;
    glm::mat4 transform;

    Shaders* shader;

public:
    Cube();
    ~Cube();

    void draw(Camera* camera);
    void load(const char* texturePath, glm::mat4 transformMatrix);
    void destroy();

};