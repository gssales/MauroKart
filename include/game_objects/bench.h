#ifndef _BENCH_H
#define _BENCH_H

#include <string>
#include <algorithm>
#include "gamestate.h"
#include "camera.h"

class BenchShader: public GpuProgram
{
public:
    Texture bench_texture;
    GLint n_lights_uniform;
    GLint light_positions_uniform;
    GLint light_colors_uniform;
    GLint texture0_uniform;

    BenchShader() {};
    BenchShader(bool build);
};

class Bench: public GameObject
{
public:
    BenchShader shader;
    Camera camera;
    std::string model_name;
    float scale = 1.0f;

    Bench();
    void Update(double dt);
    void Render(glm::mat4* model, glm::mat4* view, glm::mat4* projection, GpuProgram* default_shader, LightSet* lighting);
    void Destroy();
};

#endif // _BENCH_H
