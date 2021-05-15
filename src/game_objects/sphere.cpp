#include "game_objects/sphere.h"

SphereShader::SphereShader(const char* vertex_shader_filename, const char* fragment_shader_filename)
    : GpuProgram(vertex_shader_filename, fragment_shader_filename)
{
    n_lights_uniform = glGetUniformLocation(program_id, "n_lights");
    light_positions_uniform = glGetUniformLocation(program_id, "light_positions[]");
    light_colors_uniform = glGetUniformLocation(program_id, "light_colors[]");
}

Sphere::Sphere() : GameObject()
{
    model_name = "sphere";
    if (default_vs_filename.c_str()) {
        shader = SphereShader(default_vs_filename.c_str(), "../../res/shaders/sphere_fs.glsl");
    }

    position = glm::vec3(0.0,0.0,0.0);

    light1.color = glm::vec3(1.0f, 0.0f, 0.0f);
    light2.color = glm::vec3(0.0f, 0.0f, 1.0f);
}

void Sphere::Update(double dt) {
    light1.position = glm::vec4(position.x, position.y, position.z-2.0, 1.0f);
    light2.position = glm::vec4(position.x+2.0, position.y, position.z, 1.0f);
}

void Sphere::Render(glm::mat4* model, glm::mat4* view, glm::mat4* projection, GpuProgram* default_shader, LightSet* lighting)
{
    *model = *model * Matrix_Translate(position.x, position.y, position.z);
    PushMatrix(*model);
        *model = *model
              * Matrix_Rotate_Z(0.0)
              * Matrix_Rotate_Y(0.0)
              * Matrix_Rotate_X(0.0);
        PushMatrix(*model);
            *model = *model * Matrix_Scale(1.0, 1.0, 1.0);
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

void Sphere::Destroy() {

}
