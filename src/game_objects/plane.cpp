#include "game_objects/plane.h"

Plane::Plane() : GameObject()
{
    printf("Plane load started %i\n", creation_time);
    model_name = "plane";
    shape_type = PLANE_SHAPE;
    if (default_vs_filename.c_str()) {
        shader = CubeShader(default_vs_filename.c_str(), "../../res/shaders/cube_fs.glsl");
    }

    position = glm::vec3(0.0,0.0,0.0);
    printf("Plane load finished\n");
}

void Plane::Update(double dt) 
{
    if (input.GetKeyState(GLFW_KEY_Z).is_down) {
        s += (3.1415/6) * dt;
    }
    
    if (input.GetKeyState(GLFW_KEY_X).is_down) {
        s -= std::max((3.1415/6) * dt, std::numeric_limits<double>::epsilon());
    }
    
    if (input.GetKeyState(GLFW_KEY_1).is_down) {
        rx += (3.1415/6) * dt;
    }
    
    if (input.GetKeyState(GLFW_KEY_2).is_down) {
        ry += (3.1415/6) * dt;
    }
    
    if (input.GetKeyState(GLFW_KEY_3).is_down) {
        rz += (3.1415/6) * dt;
    }
    
    if (input.GetKeyState(GLFW_KEY_R).is_down) {
        rz = 0.0;
        ry = 0.0;
        rx = 0.0;
        s = 1.0;
    }
}

void Plane::Render(glm::mat4* model, glm::mat4* view, glm::mat4* projection, GpuProgram* default_shader, LightSet* lighting)
{
    // printf("Render iniciado %f %f %f\n", position.x, position.y, position.z);
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

void Plane::Destroy() {

}

glm::mat4 Plane::ComputeTransform() {
    return Matrix_Identity() 
        * Matrix_Translate(position.x, position.y, position.z)
              * Matrix_Rotate_Z(rz)
              * Matrix_Rotate_Y(ry)
              * Matrix_Rotate_X(rx)
              * Matrix_Scale(s,s,s);
}

SphereShape Plane::GetSphereShape()
{
    glm::mat4 transform = ComputeTransform();
    SphereShape s;
    s.point = transform * s.point;
    s.radius = 1.0f;
    return s;
}

OBBShape Plane::GetOBBShape()
{
    OBBShape s;
    return s;
}

PlaneShape Plane::GetPlaneShape()
{
    glm::mat4 transform = ComputeTransform();
    PlaneShape s;
    s.point = transform * s.point;
    s.normal = normalize(transform * s.normal);
    for (int i = 0; i < 4; i++)
        s.limits[i] = transform * s.limits[i];
    // Print_PlaneShape(s);
    return s;
}
