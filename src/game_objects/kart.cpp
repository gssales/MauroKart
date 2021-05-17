#include "game_objects/kart.h"

KartShader::KartShader(bool build)
    : GpuProgram(default_vs_filename.c_str(), "../../res/shaders/kart_fs.glsl")
{
    kart_texture0 = Texture("../../res/textures/kart1.png", 2);
    n_lights_uniform = glGetUniformLocation(program_id, "n_lights");
    light_positions_uniform = glGetUniformLocation(program_id, "light_positions[]");
    light_colors_uniform = glGetUniformLocation(program_id, "light_colors[]");
    texture0_uniform = glGetUniformLocation(program_id, "TextureImage0");
}

WheelShader::WheelShader(bool build)
    : GpuProgram(default_vs_filename.c_str(), "../../res/shaders/kart_fs.glsl")
{
    wheel_texture0 = Texture("../../res/textures/wheel.png", 4);
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
        wheel_shader = WheelShader(true);
    }

    position = glm::vec4(0.0,0.4,0.0,1.0);

    movement_vec = glm::vec4(0.0,0.0,0.0,0.0);
    speed = 0.0f;
    acceleration = 0.2f;
    max_speed = 1.0f;

    camera.farplane = 100.0f;
}

void Kart::Update(double dt)
{
    cooldown_camera = std::max(cooldown_camera-dt, 0.0);
    if (cooldown_camera <= 0 && input.GetKeyState(GLFW_KEY_V).is_down)
    {
        camera.free = !camera.free;
        cooldown_camera = 1.0;
    }

    bool is_turning = false;
    if (input.GetKeyState(GLFW_KEY_A).is_down)
    {
        is_turning = true;
        float new_ry = ry + (3.1415/4)*dt;
        if (new_ry >= 2*3.1415)
            ry = 0.0;
        else
            ry = new_ry;
        turning = 1.0;
    }
    if (input.GetKeyState(GLFW_KEY_D).is_down)
    {
        is_turning = true;
        float new_ry = ry - (3.1415/4)*dt;
        if (new_ry <= 0.0)
            ry = 2*3.1415;
        else
            ry = new_ry;
        turning = -1.0;
    }
    if (!is_turning)
        turning = 0.0;

    if (input.GetKeyState(GLFW_KEY_W).is_down)
    {
        speed = std::min((float)(speed + acceleration*dt), max_speed);
    }
    if (input.GetKeyState(GLFW_KEY_S).is_down)
    {
        speed = std::max((float)(speed - 0.5*dt), -max_speed);
    }
    if (!input.GetKeyState(GLFW_KEY_W).is_down && !input.GetKeyState(GLFW_KEY_S).is_down)
        if (speed >= 0)
            speed = std::max((float)(speed - 0.5*dt), 0.0f);
        else
            speed = std::min((float)(speed + 0.5*dt), 0.0f);

    glm::mat4 transform = Matrix_Identity() * Matrix_Rotate_Y(ry);
    movement_vec = transform * glm::vec4(0.0,0.0,-1.0,0.0);

    if (!touch_ground)
        movement_vec += glm::vec4(0.0,-1.0,0.0,0.0);
    else
        movement_vec.y = 0.0;

    position = position + (movement_vec * speed);

    if (camera.free)
    {
        glm::mat4 rotate = Matrix_Identity() * Matrix_Rotate_Y(2*3.1415 + ry);
        glm::vec4 camera_pos = position + rotate * glm::vec4(0.0, 1.0, 1.0, 0.0);
        camera.position = position + rotate * glm::vec4(0.0, 1.0, 1.0, 0.0);
        // camera.view_vector = rotate * glm::vec4(0.0, 0.0, -1.0, 0.0);
        camera.theta = 3.1415 + ry; 
    }
    else
    {
        camera.lookat = glm::vec4(position.x, position.y, position.z, 1.0);
        camera.theta = ry; 
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
        
        glUseProgram(wheel_shader.program_id);
        glUniform1i(wheel_shader.texture0_uniform          , wheel_shader.wheel_texture0.texture_unit);
        glUniformMatrix4fv(wheel_shader.view_uniform       , 1 , GL_FALSE , glm::value_ptr(*view));
        glUniformMatrix4fv(wheel_shader.projection_uniform , 1 , GL_FALSE , glm::value_ptr(*projection));
        glUniform1i(wheel_shader.n_lights_uniform          , lighting->n_lights);
        glUniform4fv(wheel_shader.light_positions_uniform  , lighting->n_lights , glm::value_ptr(lighting->positions[0]));
        glUniform3fv(wheel_shader.light_colors_uniform     , lighting->n_lights , glm::value_ptr(lighting->colors[0]));

        PushMatrix(*model);
            *model = *model * Matrix_Translate(-0.8, 0.0, -0.4);
            *model = *model * Matrix_Rotate_Y(0.5 * turning);
            *model = *model * Matrix_Scale(0.75, 0.75, 0.75);
            glUniformMatrix4fv(wheel_shader.model_uniform      , 1 , GL_FALSE , glm::value_ptr(*model));
            DrawVirtualObject("wheel", wheel_shader.program_id);
        PopMatrix(*model);
        PushMatrix(*model);
            *model = *model * Matrix_Translate(0.8, 0.0, -0.4);
            *model = *model * Matrix_Rotate_Y(0.5 * turning);
            *model = *model * Matrix_Scale(0.75, 0.75, 0.75);
            glUniformMatrix4fv(wheel_shader.model_uniform      , 1 , GL_FALSE , glm::value_ptr(*model));
            DrawVirtualObject("wheel", wheel_shader.program_id);
        PopMatrix(*model);

        PushMatrix(*model);
        *model = *model * Matrix_Translate(-0.8, 0.0, 1.6);
            glUniformMatrix4fv(wheel_shader.model_uniform      , 1 , GL_FALSE , glm::value_ptr(*model));
            DrawVirtualObject("wheel", wheel_shader.program_id);
        PopMatrix(*model);
        PushMatrix(*model);
        *model = *model * Matrix_Translate(0.8, 0.0, 1.6);
            glUniformMatrix4fv(wheel_shader.model_uniform      , 1 , GL_FALSE , glm::value_ptr(*model));
            DrawVirtualObject("wheel", wheel_shader.program_id);
        PopMatrix(*model);

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
    s.center = transform * glm::vec4(0.0,0.5,0.0,1.0);
    s.axis.x = normalize(transform * s.axis.x);
    s.axis.y = normalize(transform * s.axis.y);
    s.axis.z = normalize(transform * s.axis.z);
    s.rotation.alpha = rx;
    s.rotation.beta = ry;
    s.rotation.gamma = rz;
    s.half_length.x = 2.0;
    s.half_length.y = 1.0;
    s.half_length.z = 1.2;
    return s;
}

PlaneShape Kart::GetPlaneShape()
{
    PlaneShape s;
    return s;
}
