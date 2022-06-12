#include "shader.hpp"


Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexString, fragmentString;
    std::ifstream vertexFile, fragmentFile;
    // ensure ifstream objects can throw exceptions:
    vertexFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    
    try
    {
        vertexFile.open(vertexPath,std::ifstream::in);
        fragmentFile.open(fragmentPath,std::ifstream::in);

        std::stringstream tmp;
        tmp << vertexFile.rdbuf();
        vertexString = tmp.str();

        tmp.str( std::string() );
        tmp.clear();

        tmp << fragmentFile.rdbuf();
        fragmentString = tmp.str();

    }
    catch(std::ifstream::failure e)
    {
        std::cout << "Error while reading shader file\n";
    }

    const char *vertexCode = vertexString.c_str();
    const char *fragmentCode = fragmentString.c_str();

    int success;
    char infoLog[512];

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexCode, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}


void Shader::use()
{
    glUseProgram(ID);
}

// utility uniform functions
void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID,name.c_str()),value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID,name.c_str()),value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID,name.c_str()),value);
}
