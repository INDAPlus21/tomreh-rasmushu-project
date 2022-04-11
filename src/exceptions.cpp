#include "exceptions.h"

#include <GL/glew.h>
#include <iostream>

void i_exceptions_opengl_clear_errors()
{
	while (glGetError() != GL_NO_ERROR);
}

void i_exceptions_opengl_check_error(const char* file, int line)
{
	GLenum error = glGetError();

	bool error_found = false;
	while (error != GL_NO_ERROR)
	{
		std::cout << "OpenGL Error (" << file << ", line: " << line << "): code = " << error << std::endl;
		error_found = true;
		
		error = glGetError();
	}

	if (error_found)
	{
		std::cout << " - Press enter to close program - " << std::endl;
		std::cin.get();
		exit(1);
	}
}