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
    model_name = "crystal";
    shape_type = SPHERE_SHAPE;
    if (default_vs_filename.c_str()) {
        shader = GpuProgram(default_vs_filename.c_str(), "../../res/shaders/crystal_fs.glsl");
    }

    position = glm::vec4(0.0,0.0,0.0,1.0);

    light.color = glm::vec3(1.0f, 0.843f, 0.0f);
}

void Cube::Update(double dt) 
{
    if (dead) return;
    light.position = position;
    
    ry = ry + (3.1415/6)*dt;
    rx = rx + (3.1415/12)*dt;
    rz = rz + (3.1415/8)*dt;

    if (grow){
        s += 0.1*dt;
        grow = s <= 1.5;
    } else {
        s -= 0.1*dt;
        grow = s > 0.5;
    }
}

void Cube::Render(glm::mat4* model, glm::mat4* view, glm::mat4* projection, GpuProgram* default_shader, LightSet* lighting)
{
    if (dead) return;
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
    s.radius = this->s;
    return s;
}

OBBShape Cube::GetOBBShape()
{
    OBBShape s;
    return s;
}

PlaneShape Cube::GetPlaneShape()
{
    PlaneShape s;
    return s;
}