#include "game_objects/kart.h"

KartShader::KartShader(bool build)
    : GpuProgram(default_vs_filename.c_str(), "../../res/shaders/kart_fs.glsl")
{
    kart_texture0 = Texture("../../res/textures/kart1.png", 1);
    n_lights_uniform = glGetUniformLocation(program_id, "n_lights");
    light_positions_uniform = glGetUniformLocation(program_id, "light_positions[]");
    light_colors_uniform = glGetUniformLocation(program_id, "light_colors[]");
    texture0_uniform = glGetUniformLocation(program_id, "TextureImage0");
}

Kart::Kart() : GameObject()
{
    model_name = "kart";

    if (default_vs_filename.c_str()) {
        shader = KartShader(true);
    }

    position = glm::vec3(0.0,0.0,-2.0);

    movement_vec = glm::vec3(0.0,0.0,0.0);
    speed = 0.0f;
    acceleration = 0.1f;
    max_speed = 0.05f;
}

void Kart::Update(double dt)
{
    camera.lookat = glm::vec4(position.x, position.y, position.z, 1.0);
    bool accel = false;
    movement_vec = glm::vec3(0.0,0.0,0.0);
    if (input.GetKeyState(GLFW_KEY_W).is_down) {
        accel = true;
        movement_vec = movement_vec + glm::vec3(0.0,0.0,-1.0);
    }
    if (input.GetKeyState(GLFW_KEY_A).is_down) {
        accel = true;
        movement_vec = movement_vec + glm::vec3(-1.0,0.0,0.0);
    }
    if (input.GetKeyState(GLFW_KEY_S).is_down) {
        accel = true;
        movement_vec = movement_vec + glm::vec3(0.0,0.0,1.0);
    }
    if (input.GetKeyState(GLFW_KEY_D).is_down) {
        accel = true;
        movement_vec = movement_vec + glm::vec3(1.0,0.0,0.0);
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
}

void Kart::Render(glm::mat4* model, glm::mat4* view, glm::mat4* projection, GpuProgram* default_shader, LightSet* lighting)
{
    *model = *model * Matrix_Translate(position.x, position.y, position.z);
    camera.position = *model * camera.position;
    PushMatrix(*model);
        *model = *model
              * Matrix_Rotate_Z(0.0)
              * Matrix_Rotate_Y(0.0)
              * Matrix_Rotate_X(0.0);
        PushMatrix(*model);
            *model = *model * Matrix_Scale(1.0, 1.0, 1.0);
            glUseProgram(shader.program_id);
            glUniform1i(shader.texture0_uniform          , shader.kart_texture0.texture_unit);
            glUniformMatrix4fv(shader.view_uniform       , 1 , GL_FALSE , glm::value_ptr(*view));
            glUniformMatrix4fv(shader.projection_uniform , 1 , GL_FALSE , glm::value_ptr(*projection));
            glUniformMatrix4fv(shader.model_uniform      , 1 , GL_FALSE , glm::value_ptr(*model));
            glUniform1i(shader.n_lights_uniform          , lighting->n_lights);
            glUniform4fv(shader.light_positions_uniform  , lighting->n_lights , glm::value_ptr(lighting->positions[0]));
            glUniform3fv(shader.light_colors_uniform     , lighting->n_lights , glm::value_ptr(lighting->colors[0]));
            DrawVirtualObject(model_name.c_str(), shader.program_id);
        PopMatrix(*model);

        // std::list<BodyPart>::iterator it = part.children.begin();
        // while (it != part.children.end())
        // {
        //     PushMatrix(model);
        //         ModelBodyPartMatrix(*it, model, model_uniform, render_as_black_uniform);
        //     PopMatrix(model);
        //     it++;
        // }
    PopMatrix(*model);
}

void Kart::Destroy()
{

}