#ifndef _LOADERS_H
#define _LOADERS_H

#include <sstream>
#include <fstream>
#include <glad/glad.h>

unsigned int LoadShader(const char* filename, unsigned int shader_id);
GLuint LoadShader_Vertex(const char* filename);
GLuint LoadShader_Fragment(const char* filename);

#endif // _LOADERS_H
