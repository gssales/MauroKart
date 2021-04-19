#ifndef _CUBE_H
#define _CUBE_H

#include <string>
#include "gamestate.h"

class Cube: public GameObject
{
public:
    GpuProgram shader;
    Light light1, light2;
    std::string model_name;
    std::string fragment_shader;

    Cube();
    void Update(double dt);
    void Render(glm::mat4* model, glm::mat4* view, glm::mat4* projection, GpuProgram* default_shader, LightSet* lighting);
    void Destroy();
};

#endif // _CUBE_H