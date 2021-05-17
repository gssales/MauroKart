#include "game_objects/sphere.h"

Sphere::Sphere() : GameObject()
{
    printf("Sphere load started %i\n", creation_time);
    model_name = "sphere";
    shape_type = SPHERE_SHAPE;
    if (default_vs_filename.c_str()) {
        shader = CubeShader(default_vs_filename.c_str(), "../../res/shaders/cube_fs.glsl");
    }

    position = glm::vec3(0.0,0.0,0.0);
    printf("Sphere load finished\n");
}

void Sphere::Update(double dt) {
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
