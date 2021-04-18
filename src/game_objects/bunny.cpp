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
    camera.lookat = glm::vec4(position.x, position.y, position.z, 1.0);
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
    
    if (input.GetKeyState(GLFW_KEY_E).is_down) {
        ry += (3.1415/6) * dt;
    }
    if (input.GetKeyState(GLFW_KEY_Q).is_down) {
        ry -= (3.1415/6) * dt;
    }
    // printf("%f %f %f\n", position.x, position.y, position.z);

    // camera.Update();
}

void Bunny::Render(glm::mat4 model) {

    model = model * Matrix_Translate(position.x, position.y, position.z);
    PushMatrix(model);
        model = model
              * Matrix_Rotate_Z(0.0)
              * Matrix_Rotate_Y(ry)
              * Matrix_Rotate_X(0.0);
        PushMatrix(model);
            model = model * Matrix_Scale(1.0, 1.0, 1.0);
                    glUniformMatrix4fv(model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
                    LoadGpuProgram(default_vs_filename.c_str(), fragment_shader.c_str());
                    DrawVirtualObject(model_name.c_str());
        PopMatrix(model);

        // std::list<BodyPart>::iterator it = part.children.begin();
        // while (it != part.children.end())
        // {
        //     PushMatrix(model);
        //         ModelBodyPartMatrix(*it, model, model_uniform, render_as_black_uniform);
        //     PopMatrix(model);
        //     it++;
        // }
    PopMatrix(model);
}

void Bunny::Destroy() {

}
