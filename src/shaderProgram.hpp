
#pragma once

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
  

/*---------------------------------------------------------------
 * SHADER_PROGRAM CLASS HANDLES VERTEX AND FRAGMENT SHADERS
 *---------------------------------------------------------------
 */

class ShaderProgram
{

public:

    // program id
    unsigned int ID;
  
    // constructor reads and builds shaders
    ShaderProgram(const char* vertexPath, const char* fragmentPath);
    ~ShaderProgram();

    // destroys shader program
    void destroy();

    // activate shader
    void use();

    // utility uniform functions
    void setBool(const char* name, bool value) const;  
    void setInt(const char* name, int value) const;   
    void setFloat(const char* name, float value) const;
    void setVec3(const char* name, glm::vec3 value) const;
    void setMat4(const char* name, glm::mat4 value) const;
};
