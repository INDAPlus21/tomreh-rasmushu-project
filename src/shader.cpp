#include "renderer.h"
#include "shader.h"

#include <iostream>
#include <fstream>
#include <vector>

std::string readFile(const char *path)
{
    std::string content;
    std::ifstream fileStream(path, std::ios::in);

    if (!fileStream.is_open())
    {
        std::cerr << "Could not read file " << path << ". Files does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

GLuint createProgram(const char* VertexPath, const char* FragPath)
{
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertShaderStr = readFile(VertexPath);
    std::string fragShaderStr = readFile(FragPath);
    const char *vertShaderSrc = vertShaderStr.c_str();
    const char *fragShaderSrc = fragShaderStr.c_str();

    GLint result = GL_FALSE;
    int logLength;

    std::cout << "Compliling vertex shader." << std::endl;
    glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
    glCompileShader(vertShader);

    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> vertShaderError((logLength > 1) ? logLength : 1);
    glGetShaderInfoLog(fragShader, logLength, NULL, &vertShaderError[0]);
    std::cout << &vertShaderError[0] << std::endl;

    std::cout << "Compiling fragment shader." << std::endl;
    glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
    glCompileShader(fragShader);
    
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> fragShaderError((logLength > 1) ? logLength : 1);
    glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);
    std::cout << &fragShaderError[0] << std::endl;

    std::cout << "Linking program" << std::endl;
    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> programError((logLength > 1) ? logLength : 1);
    glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
    std::cout << &programError[0] << std::endl;

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return program;
}

void deleteProgram(GLuint *id)
{
    glDeleteProgram(*id);
}

void addFloatUniform(uint32_t program, const char* name, uint32_t num, float* vals)
{
    //TODO: Add size check
    glUseProgram(program);
    int location = glGetUniformLocation(program, name);
    switch (num)
    {
    case 1:
        glUniform1f(location, vals[0]);
        break;
    case 2:
        glUniform2f(location, vals[0], vals[1]);
        break;
    case 3:
        glUniform3f(location, vals[0], vals[1], vals[2]);
        break;
    case 4:
        glUniform4f(location, vals[0], vals[1], vals[2], vals[3]);
        break;
    }
    glUseProgram(0);
}

void addIntUniform(uint32_t program, const char* name, uint32_t num, int* vals)
{
    //TODO: Add size check
    glUseProgram(program);
    int location = glGetUniformLocation(program, name);
    switch (num)
    {
    case 1:
        glUniform1i(location, vals[0]);
        break;
    case 2:
        glUniform2i(location, vals[0], vals[1]);
        break;
    case 3:
        glUniform3i(location, vals[0], vals[1], vals[2]);
        break;
    case 4:
        glUniform4i(location, vals[0], vals[1], vals[2], vals[3]);
        break;
    }
    glUseProgram(0);
}