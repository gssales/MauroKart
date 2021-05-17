#ifndef _SPHERE_H
#define _SPHERE_H

#include <string>
#include "gamestates/gamestate.h"
#include "graphics/camera.h"

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
    Light light1, light2, light3;
    std::string model_name;
    std::string fragment_shader;
    glm::vec3 movement_vec;
    glm::vec3 bezier_p1;
    glm::vec3 bezier_p2;
    glm::vec3 bezier_p3;
    glm::vec3 bezier_p4;
    float time_passed;

    Sphere();
    void Update(double dt);
    void Render(glm::mat4* model, glm::mat4* view, glm::mat4* projection, GpuProgram* default_shader, LightSet* lighting);
    void Destroy();
    glm::mat4 ComputeTransform();
    SphereShape GetSphereShape();
    OBBShape GetOBBShape();
    PlaneShape GetPlaneShape();
};

#endif // _SPHERE_H
