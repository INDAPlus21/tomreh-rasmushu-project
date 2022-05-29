#pragma once
#include <string>
#include <cinttypes>

std::string readFile(const char *path);
GLuint createProgram(const char* VertexPath, const char* FragPath);
void deleteProgram(GLuint *id);

void addFloatUniform(uint32_t program, const char* name, uint32_t num, float* vals);
void addIntUniform(uint32_t program, const char* name, uint32_t num, int* vals);
