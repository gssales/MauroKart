#ifndef _CUBE_H
#define _CUBE_H

#include <string>
#include "gamestates/gamestate.h"
#include "graphics/camera.h"

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
    Light light1, light2, light3;
    std::string model_name;
    float rx = 0.0f;
    float ry = 0.0f;
    float rz = 0.0f;
    float s = 1.0f;

    Cube();
    void Update(double dt);
    void Render(glm::mat4* model, glm::mat4* view, glm::mat4* projection, GpuProgram* default_shader, LightSet* lighting);
    void Destroy();
    glm::mat4 ComputeTransform();
    SphereShape GetSphereShape();
    OBBShape GetOBBShape();
    PlaneShape GetPlaneShape();
};

#endif // _CUBE_H