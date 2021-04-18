#ifndef _BUNNY_H
#define _BUNNY_H

#include <string>
#include <algorithm>
#include "gamestate.h"
#include "camera.h"

class Bunny: public GameObject
{
public:
    Camera camera;
    std::string model_name;
    std::string fragment_shader;
    glm::vec3 movement_vec;
    float speed;
    float acceleration;
    float max_speed;
    float ry = 0.0f;

    // Bunny(GameState* game_state);
    Bunny();
    void Update(double dt);
    void Render(glm::mat4* model, glm::mat4* view, glm::mat4* projection);
    void Destroy();
};

#endif // _BUNNY_H