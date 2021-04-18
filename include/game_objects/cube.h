#ifndef _CUBE_H
#define _CUBE_H

#include <string>
#include "gamestate.h"

class Cube: public GameObject
{
public:
    std::string model_name;
    std::string fragment_shader;

    Cube();
    void Update(double dt);
    void Render(glm::mat4 model);
    void Destroy();
};

#endif // _CUBE_H