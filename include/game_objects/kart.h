#ifndef _KART_H
#define _KART_H

#include <string>
#include "gamestate.h"

class KartShader: public GpuProgram
{
public:
    Texture kart_texture0;
    GLint n_lights_uniform;
    GLint light_positions_uniform;
    GLint light_colors_uniform;
    GLint texture0_uniform;

    KartShader() {};
    KartShader(bool build);
};

class Kart: public GameObject
{
public:
    KartShader shader;
    std::string model_name;

    Kart();
    void Update(double dt);
    void Render(glm::mat4* model, glm::mat4* view, glm::mat4* projection, GpuProgram* default_shader, LightSet* lighting);
    void Destroy();
};

#endif // _KART_H