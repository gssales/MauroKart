#include "game_objects/sphere.h"
#include <cmath>

using std::string;

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
    shape_type = SPHERE_SHAPE;

    if (default_vs_filename.c_str()) {
        shader = SphereShader(default_vs_filename.c_str(), "../../res/shaders/sphere_fs.glsl");
    }

    position = glm::vec4(-2.0,0.0,-30.0,1.0);
    time_passed = 0.0f;

    movement_vec = glm::vec4(0.0,0.0,0.0,0.0);
    
    float bezier_0 = 10.0;
    float bezier_1 = 12.5;
    bezier_p1 = position + glm::vec4(-bezier_0, 1.0, 0.0,1.0);
    bezier_p2 = position + glm::vec4(-bezier_0, bezier_1, 0.0,1.0);
    bezier_p3 = position + glm::vec4(bezier_0, bezier_1, 0.0,1.0);
    bezier_p4 = position + glm::vec4(bezier_0, 1.0, 0.0,1.0);

    light1.color = glm::vec3(0.0f, 1.0f, 0.0f);
    light2.color = glm::vec3(1.0f, 0.0f, 0.0f);
    light3.color = glm::vec3(0.0f, 0.0f, 1.0f);
}

void Sphere::Update(double dt) {
    light1.position = glm::vec4(position.x+2.0, position.y, position.z, 1.0f);
    light2.position = glm::vec4(position.x, position.y+2.0, position.z, 1.0f);
    light3.position = glm::vec4(position.x, position.y, position.z+2.0, 1.0f);

    time_passed += dt;
    float sinusoidal_time_passed = (sin(time_passed) + 1) * 0.5;

    glm::vec3 term1 = ((float) pow((1 - sinusoidal_time_passed), 3)) * bezier_p1;
    glm::vec3 term2 = 3 * ((float) pow((1 - sinusoidal_time_passed), 2)) * sinusoidal_time_passed * bezier_p2;
    glm::vec3 term3 = 3 * (1 - sinusoidal_time_passed) * ((float) pow(sinusoidal_time_passed, 2)) * bezier_p3;
    glm::vec3 term4 = ((float) pow(sinusoidal_time_passed, 3)) * bezier_p4;
    
    movement_vec = term1 + term2 + term3 + term4;
    position.x = movement_vec.x;
    position.y = movement_vec.y;
    position.z = movement_vec.z;
}

void Sphere::Render(glm::mat4* model, glm::mat4* view, glm::mat4* projection, GpuProgram* default_shader, LightSet* lighting)
{
    // printf("Render iniciado %f %f %f\n", position.x, position.y, position.z);
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

glm::mat4 Sphere::ComputeTransform() {
    return Matrix_Identity()
        * Matrix_Translate(position.x, position.y, position.z)
        * Matrix_Rotate_Z(0.0)
        * Matrix_Rotate_Y(0.0)
        * Matrix_Rotate_X(0.0)
        * Matrix_Scale(1.0, 1.0, 1.0);
}

SphereShape Sphere::GetSphereShape()
{
    glm::mat4 transform = ComputeTransform();
    SphereShape s;
    s.point = transform * s.point;
    s.radius = 1.0f;
    return s;
}

OBBShape Sphere::GetOBBShape()
{
    OBBShape s;
    return s;
}

PlaneShape Sphere::GetPlaneShape()
{
    PlaneShape s;
    return s;
}
