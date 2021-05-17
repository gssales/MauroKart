#include "game_objects/bench.h"


BenchShader::BenchShader(bool build)
    : GpuProgram(default_vs_filename.c_str(), "../../res/shaders/bench_fs.glsl")
{
    bench_texture = Texture("../../res/textures/bench.jpg", 2);
    n_lights_uniform = glGetUniformLocation(program_id, "n_lights");
    light_positions_uniform = glGetUniformLocation(program_id, "light_positions[]");
    light_colors_uniform = glGetUniformLocation(program_id, "light_colors[]");
    texture0_uniform = glGetUniformLocation(program_id, "TextureImage1");
}

Bench::Bench() : GameObject()
{
    model_name = "bench";
    shape_type = OBB_SHAPE;
    if (default_vs_filename.c_str()) {
        shader = BenchShader(true);
    }

    position = glm::vec4(0.0,0.0,0.0,1.0);
    scale = 0.02f;
}

void Bench::Update(double dt) {
    camera.lookat = position;
}

void Bench::Render(glm::mat4* model, glm::mat4* view, glm::mat4* projection, GpuProgram* default_shader, LightSet* lighting)
{
    *model = *model * Matrix_Translate(position.x, position.y, position.z);
    camera.position = *model * camera.position;
    PushMatrix(*model);
        *model = *model
              * Matrix_Rotate_Z(0.0)
              * Matrix_Rotate_Y(0.0)
              * Matrix_Rotate_X(0.0);
        PushMatrix(*model);
            *model = *model * Matrix_Scale(scale, scale, scale);
            glUseProgram(shader.program_id);
            glUniform1i(shader.texture0_uniform         , shader.bench_texture.texture_unit);
            glUniformMatrix4fv(shader.view_uniform      , 1 , GL_FALSE , glm::value_ptr(*view));
            glUniformMatrix4fv(shader.projection_uniform, 1 , GL_FALSE , glm::value_ptr(*projection));
            glUniformMatrix4fv(shader.model_uniform     , 1 , GL_FALSE , glm::value_ptr(*model));
            glUniform1i(shader.n_lights_uniform         , lighting->n_lights);
            glUniform4fv(shader.light_positions_uniform , lighting->n_lights , glm::value_ptr(lighting->positions[0]));
            glUniform3fv(shader.light_colors_uniform    , lighting->n_lights , glm::value_ptr(lighting->colors[0]));
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

void Bench::Destroy() {

}

glm::mat4 Bench::ComputeTransform() {
    return Matrix_Identity() 
        * Matrix_Translate(position.x, position.y, position.z)
        * Matrix_Rotate_Z(0.0)
        * Matrix_Rotate_Y(0.0)
        * Matrix_Rotate_X(0.0)
        * Matrix_Scale(scale, scale, scale);
}

SphereShape Bench::GetSphereShape()
{
    glm::mat4 transform = ComputeTransform();
    SphereShape s;
    s.point = transform * s.point;
    s.radius = this->scale;
    return s;
}

OBBShape Bench::GetOBBShape()
{
    glm::mat4 transform = ComputeTransform();
    OBBShape s;
    s.center = transform * s.center;
    s.axis.x = normalize(transform * s.axis.x);
    s.axis.y = normalize(transform * s.axis.y);
    s.axis.z = normalize(transform * s.axis.z);
    s.rotation.alpha = 0.0;
    s.rotation.beta = 0.0;
    s.rotation.gamma = 0.0;
    s.half_length.x = s.half_length.x * this->scale;
    s.half_length.y = s.half_length.y * this->scale;
    s.half_length.z = s.half_length.z * this->scale;
    return s;
}

PlaneShape Bench::GetPlaneShape()
{
    PlaneShape s;
    return s;
}