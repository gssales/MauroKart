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
    shape_type = OBB_SHAPE;
    if (default_vs_filename.c_str()) {
        shader = KartShader(true);
    }

    position = glm::vec3(0.0,0.0,0.0);

    movement_vec = glm::vec3(0.0,0.0,0.0);
    speed = 0.0f;
    acceleration = 0.1f;
    max_speed = 0.05f;
    
    camera.farplane = 100.0f;
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
    if (input.GetKeyState(GLFW_KEY_SPACE).is_down) {
        accel = true;
        movement_vec = movement_vec + glm::vec3(0.0,1.0,0.0);
    }
    if (input.GetKeyState(GLFW_KEY_LEFT_SHIFT).is_down) {
        accel = true;
        movement_vec = movement_vec + glm::vec3(0.0,-1.0,0.0);
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
              * Matrix_Rotate_Y(ry)
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

glm::mat4 Kart::ComputeTransform() {
    return Matrix_Identity() 
        * Matrix_Translate(position.x, position.y, position.z)
        * Matrix_Rotate_Z(rz)
        * Matrix_Rotate_Y(ry)
        * Matrix_Rotate_X(rx)
        * Matrix_Scale(1.0, 1.0, 1.0);
}

SphereShape Kart::GetSphereShape()
{
    glm::mat4 transform = ComputeTransform();
    SphereShape s;
    s.point = s.point * transform;
    s.radius = std::sqrt(2.0);
    return s;
}

OBBShape Kart::GetOBBShape()
{
    glm::mat4 transform = ComputeTransform();
    OBBShape s;
    s.center = transform * s.center;
    s.axis.x = normalize(transform * s.axis.x);
    s.axis.y = normalize(transform * s.axis.y);
    s.axis.z = normalize(transform * s.axis.z);
    s.rotation.alpha = rx;
    s.rotation.beta = ry;
    s.rotation.gamma = rz;
    s.half_length.x = 2.0;
    s.half_length.y = 1.5;
    s.half_length.z = 1.2;
    return s;
}

PlaneShape Kart::GetPlaneShape()
{
    PlaneShape s;
    return s;
}
