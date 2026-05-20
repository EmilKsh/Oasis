#include"shader.h"

#include <utility>

Shader::Shader()
{
    //Init();
}

Shader::~Shader()
{
    Release();
}

Shader::Shader(Shader&& other) noexcept
{
    ID = std::exchange(other.ID, 0);
    vertex = std::exchange(other.vertex, 0);
    fragment = std::exchange(other.fragment, 0);
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other)
    {
        Release();
        ID = std::exchange(other.ID, 0);
        vertex = std::exchange(other.vertex, 0);
        fragment = std::exchange(other.fragment, 0);
    }

    return *this;
}

bool Shader::Init(string shaderFilePath)
{
    Release();
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(shaderFilePath + ".vs");
        fShaderFile.open(shaderFilePath + ".fs");
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << shaderFilePath << std::endl;
        return false;
    }

    vShaderCode = vertexCode.c_str();
    fShaderCode = fragmentCode.c_str();

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    glUseProgram(ID);
    /*unsigned int transLocation = glGetUniformLocation(this->ID, "transform");
    glUniformMatrix4fv(transLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));*/
    return true;
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setBool(const string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::set3fv(const string& name, glm::vec3 value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}
void Shader::set4mat(const string& name, glm::mat4 mat) 
{ 
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::checkCompileErrors(int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void Shader::Release()
{
    if (ID != 0)
    {
        glDeleteProgram(ID);
        ID = 0;
    }
}
