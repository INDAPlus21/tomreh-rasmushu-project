#pragma once
#include <string>
#include <cinttypes>

std::string ReadFile(const char *path);
GLuint CreateProgram(const char* VertexPath, const char* FragPath);
void DeleteProgram(GLuint *id);

void addFloatUniform(uint32_t program, const char* name, uint32_t num, float* vals);
void addIntUniform(uint32_t program, const char* name, uint32_t num, uint32_t* vals);
