#ifndef _GAME_H
#define _GAME_H

#include <cstdlib>
#include <algorithm>
#include "graphics/camera.h"
#include "gamestates/gamestate.h"
#include "scene.h"
#include "game_objects/bunny.h"
#include "game_objects/sphere.h"
#include "game_objects/kart.h"
#include "game_objects/bench.h"
#include "game_objects/man.h"
#include "game_objects/plane.h"
#include "physics/collisions.h"

class Game : public GameState {
public:
    std::map<std::string, GameObject*> game_objects;
    GpuProgram default_shader;
    LightSet lighting;
    Kart kart;
    Bunny bunny;
    Sphere sphere;
    Bench bench;
    Man man;
    Plane plane;

    Cube cube1, cube2, cube3, cube4, cube5;

    Camera* active_cam;
    double g_LastCursorPosX, g_LastCursorPosY;

    void Load();
    void Update(double dt);
    void Render();
};

#endif // _GAME_H
