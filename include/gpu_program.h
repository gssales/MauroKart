#ifndef _GPU_PROGRAM_H
#define _GPU_PROGRAM_H

#include <map>
#include <string>
#include <glad/glad.h>
#include "loaders.h"

extern std::map<std::string, GLuint> compiled_shaders;
extern std::string default_vs_filename;
extern std::string default_fs_filename;
extern GLuint vertex_shader_id;
extern GLuint fragment_shader_id;
extern GLuint program_id;
extern GLint model_uniform;
extern GLint view_uniform;
extern GLint projection_uniform;

void LoadGpuProgram(const char* vertex_shader_filename, const char* fragment_shader_filename);
GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id);

#endif // _GPU_PROGRAM_H