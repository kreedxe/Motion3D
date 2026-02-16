
#include "shaderProgram.hpp"


/*---------------------------------------------------------------
 * CONSTRUCTOR READS AND COMPILES SHADERS
 *---------------------------------------------------------------
 */

ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath)
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


ShaderProgram::~ShaderProgram()
{
}


/*---------------------------------------------------------------
 * DESTROYS SHADER PROGRAM
 *---------------------------------------------------------------
 */

void ShaderProgram::destroy()
{
    glDeleteProgram(ID);
}

/*---------------------------------------------------------------
 * USE FUNCTION ACTIVATES SHADER PROGRAM
 *---------------------------------------------------------------
 */

void ShaderProgram::use() 
{ 
    glUseProgram(ID);
} 


/*---------------------------------------------------------------
 * UTILITY FUNCTIONS FOR WORKING WITH UNIFORMS
 *---------------------------------------------------------------
 */

void ShaderProgram::setBool(const char* name, bool value) const
{         
    glUniform1i(glGetUniformLocation(ID, name), (int)value); 
}


void ShaderProgram::setInt(const char* name, int value) const
{ 
    glUniform1i(glGetUniformLocation(ID, name), value); 
}


void ShaderProgram::setFloat(const char* name, float value) const
{ 
    glUniform1f(glGetUniformLocation(ID, name), value); 
}

void ShaderProgram::setVec3(const char* name, glm::vec3 value) const
{ 
    glUniform3fv(glGetUniformLocation(ID, name), 1, &value[0]); 
}

void ShaderProgram::setMat4(const char* name, glm::mat4 value) const
{ 
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &value[0][0]); 
}

