#pragma once

#define LOCATION __FILE__, __LINE__

#define GL_CALL(x) i_exceptions_opengl_clear_errors(); x; i_exceptions_opengl_check_error(LOCATION)

extern void i_exceptions_opengl_clear_errors();
extern void i_exceptions_opengl_check_error(const char* file, int line);