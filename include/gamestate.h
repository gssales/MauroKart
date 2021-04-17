#ifndef _GAMESTATE_H
#define _GAMESTATE_H

#include <ctime>
#include <map>
#include <string>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "matrices.h"
#include "input.h"
#include "gpu_program.h"
#include "scene.h"

extern Input input;
extern float g_ScreenRatio;

class GameState;

class GameObject
{
public:
    // GameState *game_state;
    int creation_time;
    glm::vec3 position;
    bool dead = false;

    // GameObject(GameState* game_state);
    virtual void Update(double dt) = 0;
    virtual void Render() = 0;
    virtual void Destroy() = 0;
};

class GameState
{
public:
    std::map<std::string, GameObject*> game_objects;

    void Load();
    void Update(double dt);
    void Render();
};


#endif // _GAMESTATE_H