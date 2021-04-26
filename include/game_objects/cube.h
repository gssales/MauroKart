#ifndef _CUBE_H
#define _CUBE_H

#include <string>
#include "gamestate.h"

class CubeShader: public GpuProgram
{
public:
    GLint n_lights_uniform;
    GLint light_positions_uniform;
    GLint light_colors_uniform;

    CubeShader() {};
    CubeShader(const char* vertex_shader_filename, const char* fragment_shader_filename);
};

class Cube: public GameObject
{
public:
    CubeShader shader;
    Light light1, light2;
    std::string model_name;
    std::string fragment_shader;

    Cube();
    void Update(double dt);
    void Render(glm::mat4* model, glm::mat4* view, glm::mat4* projection, GpuProgram* default_shader, LightSet* lighting);
    void Destroy();
};

#endif // _CUBE_H
