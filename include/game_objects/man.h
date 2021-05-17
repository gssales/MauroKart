#ifndef _MAN_H
#define _MAN_H

#include <string>
#include <algorithm>
#include "gamestate.h"
#include "camera.h"

class ManShader: public GpuProgram
{
public:
    GLint n_lights_uniform;
    GLint light_positions_uniform;
    GLint light_colors_uniform;

    ManShader() {};
    ManShader(bool build);
};

class Man: public GameObject
{
public:
    ManShader shader;
    Camera camera;
    std::string model_name;
    float scale = 1.0f;
    float time_passed;

    Man();
    void Update(double dt);
    void Render(glm::mat4* model, glm::mat4* view, glm::mat4* projection, GpuProgram* default_shader, LightSet* lighting);
    void Destroy();
    float MinMax(float x, float min, float max);
};

#endif // _MAN_H
