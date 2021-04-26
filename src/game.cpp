#include "game.h"

void Game::Load() {
    printf("Load iniciado\n");

    default_shader = GpuProgram(default_vs_filename.c_str(), default_fs_filename.c_str());

    ObjModel bunnymodel("../../res/models/bunny.obj");
    AddModelToScene(&bunnymodel);

    ObjModel cubemodel("../../res/models/sphere.obj");
    AddModelToScene(&cubemodel);
    
    ObjModel kartmodel("../../res/models/kart.obj");
    AddModelToScene(&kartmodel);

    cube = Cube();
    game_objects["cube"] = &cube;

    bunny = Bunny();
    game_objects["bunny"] = &bunny;

    kart = Kart();
    game_objects["kart"] = &kart;

    lighting.positions[lighting.n_lights] = cube.light1.position;
    lighting.colors[lighting.n_lights] = cube.light1.color;
    lighting.n_lights += 1;
    lighting.positions[lighting.n_lights] = cube.light2.position;
    lighting.colors[lighting.n_lights] = cube.light2.color;
    lighting.n_lights += 1;

    active_cam = &(bunny.camera);

    active_cam->free = false;
    active_cam->distance = 2.5;
    printf("load finalizado %d\n", game_objects.size());
}

void Game::Update(double dt) {
    
    lighting = LightSet();
    lighting.positions[lighting.n_lights] = cube.light1.position;
    lighting.colors[lighting.n_lights] = cube.light1.color;
    lighting.n_lights += 1;
    lighting.positions[lighting.n_lights] = cube.light2.position;
    lighting.colors[lighting.n_lights] = cube.light2.color;
    lighting.n_lights += 1;

    // printf("Update iniciado %d\n", game_objects.size());
    KeyState left_button = input.GetKeyState(GLFW_MOUSE_BUTTON_LEFT);
    if (left_button.is_pressed) {
        g_LastCursorPosX = input.cursor_state.xvalue;
        g_LastCursorPosY = input.cursor_state.yvalue;
    }
    if (left_button.is_down && !left_button.is_pressed) {
        // printf("moving camera\n");
        PairState cursor = input.cursor_state;
        float dx = cursor.xvalue - g_LastCursorPosX;
        float dy = cursor.yvalue - g_LastCursorPosY;

        active_cam->theta -= dx * dt * 0.1;
        active_cam->phi += dy * dt * 0.1;

        float phimax = 3.141592f/2;
        float phimin = -phimax;

        if (active_cam->phi > phimax)
            active_cam->phi = phimax;

        if (active_cam->phi < phimin)
            active_cam->phi = phimin;

        g_LastCursorPosX = cursor.xvalue;
        g_LastCursorPosY = cursor.yvalue;
    }
    if (input.scroll_changed) {
        // printf("scrolling camera\n");
        PairState scroll = input.scroll_state;
        // printf("scrolling camera %f\n", active_cam->distance);
        active_cam->distance -= active_cam->distance * scroll.yvalue * dt;
        // printf("scrolling camera %f\n", active_cam->distance);

        const float verysmallnumber = std::numeric_limits<float>::epsilon();
        if (active_cam->distance < verysmallnumber)
            active_cam->distance = verysmallnumber;
    }

    // bunny.Update(dt);
    // active_cam->lookat = glm::vec4(bunny.position.x, bunny.position.y, bunny.position.z, 1.0);

    active_cam->Update();

    std::map<std::string, GameObject*>::iterator it;
    for (it = game_objects.begin(); it != game_objects.end(); it++)
    {
        GameObject* game_object = it->second;
        game_object->Update(dt);

        if (game_object->dead) {
            game_object->Destroy();
            game_objects.erase(it->first);
        }
    }
}

void Game::Render() {
    // printf("Render iniciado %d\n", game_objects.size());
    glm::mat4 view = active_cam->View();
    glm::mat4 projection = active_cam->Projection(g_ScreenRatio);

    std::map<std::string, GameObject*>::iterator it;
    for (it = game_objects.begin(); it != game_objects.end(); it++)
    {
        GameObject* game_object = it->second;
        glm::mat4 model = Matrix_Identity();
        game_object->Render(&model, &view, &projection, &default_shader, &lighting);
        // printf("%s renderizado\n", it->first.c_str());
    }
}
