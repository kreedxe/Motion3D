
#include "light.hpp"
#include "glm/fwd.hpp"

#include <glad/glad.h>
#include <stb_image.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

LightCube::LightCube()
{
    m_ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    m_diffuse = glm::vec3(0.5f,0.5f, 0.5f);
    m_specular = glm::vec3(1.0f, 1.0f, 1.0f);

    m_position = glm::vec3(0.0f, 0.0f, 0.0f);
    m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
}


void LightCube::load()
{
    int success;
    char infoLog[512];

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    m_program = new ShaderProgram("../shaders/light.vert.glsl", "../shaders/light.frag.glsl");

    m_program->use();
}


LightCube::~LightCube()
{
}

void LightCube::draw(Camera* camera)
{
    glm::mat4 transform(1.0f);

    glm::mat4 T = glm::translate(glm::mat4(1.0f), m_position);
    glm::mat4 R = glm::mat4(glm::quat(glm::radians(m_rotation)));
    glm::mat4 S = glm::scale(glm::mat4(1.0f), m_scale);

    transform = T * R * S;
    
    m_program->use();

    m_program->setMat4("projection", camera->getProjection());
    m_program->setMat4("view", camera->getView());
    m_program->setMat4("model", transform);
    m_program->setVec3("lightColor", m_specular);

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
}


void LightCube::destroy()
{
    m_program->destroy();
    delete m_program;

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}


void LightCube::setPosition(glm::vec3 pos)
{
    m_position = pos;
}


void LightCube::setRotation(glm::vec3 rotation)
{
    m_rotation = rotation;
}


void LightCube::setScale(glm::vec3 scale)
{
    m_scale = scale;
}


void LightCube::move(glm::vec3 value)
{
    m_position += value;
}


void LightCube::rotate(glm::vec3 value)
{
    m_rotation += value;
}


void LightCube::scale(glm::vec3 value)
{
    m_scale *= value;
}


glm::vec3 LightCube::getPosition()
{
    return m_position;
}