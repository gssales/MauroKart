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

    ObjModel wheelmodel("../../res/models/wheel.obj");
    AddModelToScene(&wheelmodel);

    ObjModel benchmodel("../../res/models/bench.obj");
    AddModelToScene(&benchmodel);

    ObjModel boxmodel("../../res/models/box.obj");
    AddModelToScene(&boxmodel);

    ObjModel crystalmodel("../../res/models/crystal.obj");
    AddModelToScene(&crystalmodel);
    
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

    cube1 = Cube();
    cube2 = Cube();
    cube3 = Cube();
    cube4 = Cube();
    cube5 = Cube();
    game_objects["cube1"] = &cube1;
    game_objects["cube2"] = &cube2;
    game_objects["cube3"] = &cube3;
    game_objects["cube4"] = &cube4;
    game_objects["cube5"] = &cube5;

    cube1.position.x += 5.0;
    cube2.position.x += 7.0;
    cube3.position.x += 9.0;
    cube4.position.x += 11.0;
    cube5.position.x += 13.0;

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
    active_cam->phi = 3.141592f/8;
    active_cam->distance = 15.0f;

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
    
    if (!cube1.dead) {
        lighting.positions[lighting.n_lights] = cube1.light.position;
        lighting.colors[lighting.n_lights] = cube1.light.color;
        lighting.n_lights += 1;
    }
    
    if (!cube2.dead) {
        lighting.positions[lighting.n_lights] = cube2.light.position;
        lighting.colors[lighting.n_lights] = cube2.light.color;
        lighting.n_lights += 1;
    }
    
    if (!cube3.dead) {
        lighting.positions[lighting.n_lights] = cube3.light.position;
        lighting.colors[lighting.n_lights] = cube3.light.color;
        lighting.n_lights += 1;
    }
    
    if (!cube4.dead) {
        lighting.positions[lighting.n_lights] = cube4.light.position;
        lighting.colors[lighting.n_lights] = cube4.light.color;
        lighting.n_lights += 1;
    }
    
    if (!cube5.dead) {
        lighting.positions[lighting.n_lights] = cube5.light.position;
        lighting.colors[lighting.n_lights] = cube5.light.color;
        lighting.n_lights += 1;
    }

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
        float phimin = 3.141592f/8;
        if (active_cam->free) {
            phimax = phimin;
            phimin = -phimax;
        }

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


    if (!cube1.dead) {
        SphereShape crystal1 = cube1.GetSphereShape();
        bool success = Collide_OBB_Sphere(c, kart_shape, crystal1);
        if (success)
            cube1.dead = true;
    }

    if (!cube2.dead) {
        SphereShape crystal2 = cube2.GetSphereShape();
        bool success = Collide_OBB_Sphere(c, kart_shape, crystal2);
        if (success)
            cube2.dead = true;
    }

    if (!cube3.dead) {
        SphereShape crystal3 = cube3.GetSphereShape();
        bool success = Collide_OBB_Sphere(c, kart_shape, crystal3);
        if (success)
            cube3.dead = true;
    }

    if (!cube4.dead) {
        SphereShape crystal4 = cube4.GetSphereShape();
        success = Collide_OBB_Sphere(c, kart_shape, crystal4);
        if (success)
            cube4.dead = true;
    }

    if (!cube5.dead) {
        SphereShape crystal5 = cube5.GetSphereShape();
        success = Collide_OBB_Sphere(c, kart_shape, crystal5);
        if (success)
            cube5.dead = true;
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
