#include "gamestates/game.h"

void Game::Load() {
    printf("Load iniciado\n");

    default_shader = GpuProgram(default_vs_filename.c_str(), default_fs_filename.c_str());

    ObjModel bunnymodel("../../res/models/bunny.obj");
    AddModelToScene(&bunnymodel);

    ObjModel spheremodel("../../res/models/sphere.obj");
    AddModelToScene(&spheremodel);

    ObjModel kartmodel("../../res/models/kart.obj");
    AddModelToScene(&kartmodel);

    ObjModel benchmodel("../../res/models/bench.obj");
    AddModelToScene(&benchmodel);

    ObjModel boxmodel("../../res/models/box.obj");
    AddModelToScene(&boxmodel);
    
    ObjModel planemodel("../../res/models/plane.obj");
    AddModelToScene(&planemodel);

    sphere = Sphere();
    game_objects["sphere"] = &sphere;

    bunny = Bunny();
    game_objects["bunny"] = &bunny;

    kart = Kart();
    game_objects["kart"] = &kart;

    bench = Bench();
    game_objects["bench"] = &bench;

    man = Man();
    game_objects["man"] = &man;
    
    plane = Plane();
    plane.s = 100.0f;
    plane.position.y -= 1.0f;
    game_objects["plane"] = &plane;

    lighting.positions[lighting.n_lights] = sphere.light1.position;
    lighting.colors[lighting.n_lights] = sphere.light1.color;
    lighting.n_lights += 1;
    lighting.positions[lighting.n_lights] = sphere.light2.position;
    lighting.colors[lighting.n_lights] = sphere.light2.color;
    lighting.n_lights += 1;

    active_cam = &(kart.camera);

    active_cam->free = false;
    active_cam->distance = 5;
    printf("load finalizado %d\n", game_objects.size());
}

void Game::Update(double dt) {
    lighting = LightSet();
    lighting.positions[lighting.n_lights] = sphere.light1.position;
    lighting.colors[lighting.n_lights] = sphere.light1.color;
    lighting.n_lights += 1;
    lighting.positions[lighting.n_lights] = sphere.light2.position;
    lighting.colors[lighting.n_lights] = sphere.light2.color;
    lighting.n_lights += 1;

    KeyState left_button = input.GetKeyState(GLFW_MOUSE_BUTTON_LEFT);
    if (left_button.is_pressed) {
        g_LastCursorPosX = input.cursor_state.xvalue;
        g_LastCursorPosY = input.cursor_state.yvalue;
    }
    if (left_button.is_down && !left_button.is_pressed) {
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
        PairState scroll = input.scroll_state;
        active_cam->distance -= active_cam->distance * scroll.yvalue * dt;

        const float verysmallnumber = std::numeric_limits<float>::epsilon();
        if (active_cam->distance < verysmallnumber)
            active_cam->distance = verysmallnumber;
    }

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

    Contact c[2];
    bool success = false;
    PlaneShape ground_shape = plane.GetPlaneShape();
    OBBShape kart_shape = kart.GetOBBShape();

    success = Collide_OBB_Plane(c, kart_shape, ground_shape);
    if (success) {
        printf("colidiu kart x plano\n");
        Contact kart_contact = c[0];
        kart.position = kart.position + kart_contact.min_move;
        Contact plane_contact = c[1];
        // kart.movement_vec = glm::normalize(kart.movement_vec + glm::vec3(plane_contact.normal.x, plane_contact.normal.y, plane_contact.normal.z));
    }

    float kart_ray_cast_ground = Ray_Cast_Plane(kart_shape.center, -kart_shape.axis.y * (kart_shape.half_length.y + 0.1f),
            ground_shape.point, ground_shape.normal);
    if (kart_ray_cast_ground < 1.0 && kart_ray_cast_ground > 0.0)
        kart.touch_ground = true;
    else
        kart.touch_ground = false;
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
