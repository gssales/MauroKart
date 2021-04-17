#ifndef _BUNNY_H
#define _BUNNY_H

#include <string>
#include <algorithm>
#include "gamestate.h"

class Bunny: public GameObject
{
public:
    std::string model_name;
    std::string fragment_shader;
    glm::vec3 movement_vec;
    float speed;
    float acceleration;
    float max_speed;

    // Bunny(GameState* game_state);
    Bunny();
    void Update(double dt);
    void Render();
    void Destroy();
};

#endif // _BUNNY_H