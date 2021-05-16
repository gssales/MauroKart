#ifndef _CUBE_H
#define _CUBE_H

#include <string>
#include "gamestate.h"

class SphereShader: public GpuProgram
{
public:
    GLint n_lights_uniform;
    GLint light_positions_uniform;
    GLint light_colors_uniform;

    SphereShader() {};
    SphereShader(const char* vertex_shader_filename, const char* fragment_shader_filename);
};

class Sphere: public GameObject
{
public:
    SphereShader shader;
    Light light1, light2;
    std::string model_name;
    std::string fragment_shader;
    glm::vec3 movement_vec;
    glm::vec3 bezier_p1;
    glm::vec3 bezier_p2;
    glm::vec3 bezier_p3;
    float time_passed;

    Sphere();
    void Update(double dt);
    void Render(glm::mat4* model, glm::mat4* view, glm::mat4* projection, GpuProgram* default_shader, LightSet* lighting);
    void Destroy();
};

#endif // _CUBE_H
