
#include "shaders.hpp"


/*---------------------------------------------------------------
 * CONSTRUCTOR READS AND COMPILES SHADERS
 *---------------------------------------------------------------
 */

Shaders::Shaders(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);


    /*---------------------------------------------------------------
     * READ SHADER FILES
     *---------------------------------------------------------------
     */

    try 
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();		
        
        vShaderFile.close();
        fShaderFile.close();
        
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();		
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    

    /*---------------------------------------------------------------
     * COMPILE SHADERS
     *---------------------------------------------------------------
     */

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };
    
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };
    
    
    /*---------------------------------------------------------------
     * CREATE SHADER PROGRAM AND LINK SHADERS
     *---------------------------------------------------------------
     */

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}


Shaders::~Shaders()
{
}


/*---------------------------------------------------------------
 * DESTROYS SHADER PROGRAM
 *---------------------------------------------------------------
 */

void Shaders::destroy()
{
    glDeleteProgram(ID);
}

/*---------------------------------------------------------------
 * USE FUNCTION ACTIVATES SHADER PROGRAM
 *---------------------------------------------------------------
 */

void Shaders::use() 
{ 
    glUseProgram(ID);
} 


/*---------------------------------------------------------------
 * UTILITY FUNCTIONS FOR WORKING WITH UNIFORMS
 *---------------------------------------------------------------
 */

void Shaders::setBool(const std::string &name, bool value) const
{         
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
}


void Shaders::setInt(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}


void Shaders::setFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shaders::setVec3(const std::string &name, glm::vec3 value) const
{ 
    glUniform3f(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, *glm::value_ptr(value)); 
}

void Shaders::setMat4(const std::string &name, glm::mat4 value) const
{ 
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]); 
}

