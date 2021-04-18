#ifndef _GPU_PROGRAM_H
#define _GPU_PROGRAM_H

#include <map>
#include <string>
#include <glad/glad.h>
#include "loaders.h"

extern std::string default_vs_filename;
extern std::string default_fs_filename;

GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id);

class GpuProgram
{
public:
    GLuint program_id;
    GLint model_uniform;
    GLint view_uniform;
    GLint projection_uniform;

    GpuProgram() {}
    GpuProgram(const char* vertex_shader_filename, const char* fragment_shader_filename);
};

#endif // _GPU_PROGRAM_H