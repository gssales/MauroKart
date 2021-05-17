#include "game_objects/cube.h"

CubeShader::CubeShader(const char* vertex_shader_filename, const char* fragment_shader_filename)
    : GpuProgram(vertex_shader_filename, fragment_shader_filename)
{
    n_lights_uniform = glGetUniformLocation(program_id, "n_lights");
    light_positions_uniform = glGetUniformLocation(program_id, "light_positions[]");
    light_colors_uniform = glGetUniformLocation(program_id, "light_colors[]");
}

Cube::Cube() : GameObject()
{
    model_name = "cube";
    shape_type = OBB_SHAPE;
    if (default_vs_filename.c_str()) {
        shader = CubeShader(default_vs_filename.c_str(), "../../res/shaders/cube_fs.glsl");
    }

    position = glm::vec4(0.0,0.0,0.0,1.0);

    light1.color = glm::vec3(0.0f, 1.0f, 0.0f);
    light2.color = glm::vec3(1.0f, 0.0f, 0.0f);
    light3.color = glm::vec3(0.0f, 0.0f, 1.0f);
}

void Cube::Update(double dt) 
{
    light1.position = glm::vec4(position.x+2.0, position.y, position.z, 1.0f);
    light2.position = glm::vec4(position.x, position.y+2.0, position.z, 1.0f);
    light3.position = glm::vec4(position.x, position.y, position.z+2.0, 1.0f);
    
    if (input.GetKeyState(GLFW_KEY_C).is_down) {
        s += (3.1415/6) * dt;
    }

    if (input.GetKeyState(GLFW_KEY_V).is_down) {
        s -= std::max((3.1415/6) * dt, std::numeric_limits<double>::epsilon());
    }
    
    if (input.GetKeyState(GLFW_KEY_4).is_down) {
        rx += (3.1415/6) * dt;
    }
    
    if (input.GetKeyState(GLFW_KEY_5).is_down) {
        ry += (3.1415/6) * dt;
    }
    
    if (input.GetKeyState(GLFW_KEY_6).is_down) {
        rz += (3.1415/6) * dt;
    }
    
    if (input.GetKeyState(GLFW_KEY_R).is_down) {
        rz = 0.0;
        ry = 0.0;
        rx = 0.0;
        s = 1.0;
        position = glm::vec4(0.0,0.0,0.0,1.0);
        position.x = 3.0f;
    }
}

void Cube::Render(glm::mat4* model, glm::mat4* view, glm::mat4* projection, GpuProgram* default_shader, LightSet* lighting)
{
    *model = *model * Matrix_Translate(position.x, position.y, position.z);
    PushMatrix(*model);
        *model = *model
              * Matrix_Rotate_Z(rz)
              * Matrix_Rotate_Y(ry)
              * Matrix_Rotate_X(rx);
        PushMatrix(*model);
            *model = *model * Matrix_Scale(s,s,s);
            glUseProgram(shader.program_id);
            glUniformMatrix4fv(shader.view_uniform       , 1 , GL_FALSE , glm::value_ptr(*view));
            glUniformMatrix4fv(shader.projection_uniform , 1 , GL_FALSE , glm::value_ptr(*projection));
            glUniformMatrix4fv(shader.model_uniform      , 1 , GL_FALSE , glm::value_ptr(*model));
            glUniform1i(shader.n_lights_uniform , lighting->n_lights);
            glUniform4fv(shader.light_positions_uniform      , lighting->n_lights , glm::value_ptr(lighting->positions[0]));
            glUniform3fv(shader.light_colors_uniform      , lighting->n_lights , glm::value_ptr(lighting->colors[0]));
            DrawVirtualObject(model_name.c_str(), shader.program_id);
        PopMatrix(*model);
    PopMatrix(*model);
}

void Cube::Destroy() {

}

glm::mat4 Cube::ComputeTransform() {
    return Matrix_Identity() 
        * Matrix_Translate(position.x, position.y, position.z)
        * Matrix_Rotate_Z(rz)
        * Matrix_Rotate_Y(ry)
        * Matrix_Rotate_X(rx)
        * Matrix_Scale(s, s, s);
}

SphereShape Cube::GetSphereShape()
{
    glm::mat4 transform = ComputeTransform();
    SphereShape s;
    s.point = transform * s.point;
    s.radius = std::sqrt(2.0) * this->s;
    return s;
}

OBBShape Cube::GetOBBShape()
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
    s.half_length.x = s.half_length.x * this->s;
    s.half_length.y = s.half_length.y * this->s;
    s.half_length.z = s.half_length.z * this->s;
    return s;
}

PlaneShape Cube::GetPlaneShape()
{
    PlaneShape s;
    return s;
}