#include "game_objects/bunny.h"

// Bunny::Bunny(GameState* game_state) : GameObject(game_state) {
Bunny::Bunny() {
    time_t t;
    time(&t);
    creation_time = t;
    model_name = "bunny";
    fragment_shader = "../../res/shaders/bunny_fs.glsl";

    position = glm::vec3(0.0,0.0,0.0);

    movement_vec = glm::vec3(0.0,0.0,0.0);
    speed = 0.0f;
    acceleration = 0.1f;
    max_speed = 0.2f;
}

void Bunny::Update(double dt) {
    // printf("update %s\n", model_name.c_str() );
    bool accel = false;
    movement_vec = glm::vec3(0.0,0.0,0.0);
    if (input.GetKeyState(GLFW_KEY_W).is_down) {
        accel = true;
        movement_vec = movement_vec + glm::vec3(1.0,0.0,0.0);
    }
    if (input.GetKeyState(GLFW_KEY_A).is_down) {
        accel = true;
        movement_vec = movement_vec + glm::vec3(0.0,0.0,1.0);
    }
    if (input.GetKeyState(GLFW_KEY_S).is_down) {
        accel = true;
        movement_vec = movement_vec + glm::vec3(-1.0,0.0,0.0);
    }
    if (input.GetKeyState(GLFW_KEY_D).is_down) {
        accel = true;
        movement_vec = movement_vec + glm::vec3(0.0,0.0,-1.0);
    }

    if (glm::length(movement_vec) != 0.0) 
        movement_vec = glm::normalize(movement_vec);

    if (accel)
        speed = std::min((float)(speed + acceleration*dt), max_speed);
    else
        speed = std::max((float)(speed - acceleration*dt), 0.0f);

    speed = std::min((float)(speed + acceleration*dt), max_speed);
    position = position + (movement_vec * speed);
    // printf("%f %f %f\n", position.x, position.y, position.z);
}

void Bunny::Render() {
    // printf("Render iniciado %s\n", model_name.c_str());
    LoadGpuProgram(default_vs_filename.c_str(), fragment_shader.c_str());

    glm::mat4 model = Matrix_Identity() * Matrix_Translate(position.x, position.y, position.z);
    glUniformMatrix4fv(model_uniform, 1 , GL_FALSE , glm::value_ptr(model));

    // printf("draw %s\n", model_name.c_str() );
    DrawVirtualObject(model_name.c_str());
}

void Bunny::Destroy() {

}
