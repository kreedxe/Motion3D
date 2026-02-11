
#pragma once

#include <string>
#include <glm/glm.hpp>

#include "shaders.hpp"
#include "camera.hpp"

class Cube
{

private:
    unsigned int VAO, VBO, texture;
    glm::mat4 transform;

    Shaders shader;

public:
    Cube(char* texturePath, glm::mat4 transformMatrix);
    ~Cube();

    void draw(Camera* camera);
    void destroy();

};