#ifndef _GAME_H
#define _GAME_H

#include <cstdlib>
#include "camera.h"
#include "gamestate.h"
#include "scene.h"
#include "game_objects/bunny.h"
#include "game_objects/cube.h"
#include "game_objects/kart.h"

class Game : public GameState {
public:
    std::map<std::string, GameObject*> game_objects;
    GpuProgram default_shader;
    LightSet lighting;
    Kart kart;
    Bunny bunny;
    Cube cube;
    Camera* active_cam;
    double g_LastCursorPosX, g_LastCursorPosY;

    void Load();
    void Update(double dt);
    void Render();
};

#endif // _GAMESTATE_H