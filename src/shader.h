#pragma once

#include <string>

std::string ReadFile(const char *path);
GLuint CreateProgram(const char* VertexPath, const char* FragPath);
void DeleteProgram(GLuint *id);