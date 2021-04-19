#include "gpu_program.h"

std::string default_vs_filename = "../../res/shaders/shader_vertex.glsl";
std::string default_fs_filename = "../../res/shaders/shader_fragment.glsl";

GpuProgram::GpuProgram(const char* vertex_shader_filename, const char* fragment_shader_filename)
{
    GLuint fragment_shader_id = LoadShader_Fragment(fragment_shader_filename);
    GLuint vertex_shader_id = LoadShader_Vertex(vertex_shader_filename);

    if ( program_id != 0 )
        glDeleteProgram(program_id);

    program_id = CreateGpuProgram(vertex_shader_id, fragment_shader_id);

    model_uniform      = glGetUniformLocation(program_id, "model");
    view_uniform       = glGetUniformLocation(program_id, "view");
    projection_uniform = glGetUniformLocation(program_id, "projection");
    n_lights_uniform   = glGetUniformLocation(program_id, "n_lights");
    lights_uniform     = glGetUniformLocation(program_id, "lights");
    colors_uniform     = glGetUniformLocation(program_id, "colors");
}

GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id) 
{
    GLuint program_id = glCreateProgram();

    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);

    glLinkProgram(program_id);

    GLint linked_ok = GL_FALSE;
    glGetProgramiv(program_id, GL_LINK_STATUS, &linked_ok);

    if ( linked_ok == GL_FALSE )
    {
        GLint log_length = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

        GLchar* log = new GLchar[log_length];

        glGetProgramInfoLog(program_id, log_length, &log_length, log);

        std::string output;

        output += "ERROR: OpenGL linking of program failed.\n";
        output += "== Start of link log\n";
        output += log;
        output += "\n== End of link log\n";

        delete [] log;

        fprintf(stderr, "%s", output.c_str());
    }

    return program_id;
}
