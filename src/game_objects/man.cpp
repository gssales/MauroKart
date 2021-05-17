#include "game_objects/man.h"

ManShader::ManShader(bool build)
    : GpuProgram(default_vs_filename.c_str(), "../../res/shaders/sphere_fs.glsl")
{
    n_lights_uniform = glGetUniformLocation(program_id, "n_lights");
    light_positions_uniform = glGetUniformLocation(program_id, "light_positions[]");
    light_colors_uniform = glGetUniformLocation(program_id, "light_colors[]");
}

Man::Man() : GameObject()
{
    model_name = "box";

    if (default_vs_filename.c_str()) {
        shader = ManShader(true);
    }

    position = glm::vec4(-5.0,1.7,-4.5,1.0);
    scale = 0.5f;
    time_passed = 0.0f;
}

void Man::Update(double dt) {
    camera.lookat = glm::vec4(position.x, position.y, position.z, 1.0);
    time_passed += dt;
}

void Man::Render(glm::mat4* model, glm::mat4* view, glm::mat4* projection, GpuProgram* default_shader, LightSet* lighting)
{
    *model = *model * Matrix_Translate(position.x, position.y, position.z);
    camera.position = *model * camera.position;
    *model = *model * Matrix_Scale(scale, scale, scale) * Matrix_Rotate_Y(1.57079632679);
    float sinusoidal_time_passed = (sin(time_passed * 3) + 1) * 0.5;

    PushMatrix(*model);
        *model = *model * Matrix_Scale(0.8f, 1.0f, 0.2f);
        glUseProgram(shader.program_id);
        glUniformMatrix4fv(shader.view_uniform       , 1 , GL_FALSE , glm::value_ptr(*view));
        glUniformMatrix4fv(shader.projection_uniform , 1 , GL_FALSE , glm::value_ptr(*projection));
        glUniformMatrix4fv(shader.model_uniform      , 1 , GL_FALSE , glm::value_ptr(*model));
        glUniform1i(shader.n_lights_uniform , lighting->n_lights);
        glUniform4fv(shader.light_positions_uniform      , lighting->n_lights , glm::value_ptr(lighting->positions[0]));
        glUniform3fv(shader.light_colors_uniform      , lighting->n_lights , glm::value_ptr(lighting->colors[0]));
        DrawVirtualObject(model_name.c_str(), shader.program_id);
    PopMatrix(*model);

    PushMatrix(*model);
        *model = *model * Matrix_Translate(0.0f, 1.5f, 0.0f);
        *model = *model * Matrix_Rotate_X(3.141592);
        PushMatrix(*model);
            *model = *model * Matrix_Scale(0.4f, 0.4f, 0.2f);
            glUseProgram(shader.program_id);
            // glUniform1i(shader.texture0_uniform         , shader.man_texture.texture_unit);
            glUniformMatrix4fv(shader.view_uniform      , 1 , GL_FALSE , glm::value_ptr(*view));
            glUniformMatrix4fv(shader.projection_uniform, 1 , GL_FALSE , glm::value_ptr(*projection));
            glUniformMatrix4fv(shader.model_uniform     , 1 , GL_FALSE , glm::value_ptr(*model));
            glUniform1i(shader.n_lights_uniform         , lighting->n_lights);
            glUniform4fv(shader.light_positions_uniform , lighting->n_lights , glm::value_ptr(lighting->positions[0]));
            glUniform3fv(shader.light_colors_uniform    , lighting->n_lights , glm::value_ptr(lighting->colors[0]));
            DrawVirtualObject(model_name.c_str(), shader.program_id);
        PopMatrix(*model);
    PopMatrix(*model);

    PushMatrix(*model);
        *model = *model * Matrix_Translate(-1.10f, MinMax(sinusoidal_time_passed, 0.7f, 1.05f), MinMax(sinusoidal_time_passed, 0.4f, 0.0f)); // min: (x,0.7,0.4f) max (x, 1.05f, 0.0f)
        PushMatrix(*model);
            *model = *model
                    * Matrix_Rotate_Z(-0.0f)
                    * Matrix_Rotate_Y(-0.0f)
                    * Matrix_Rotate_X(MinMax(sinusoidal_time_passed, -1.57079632679f, -3.141592f));//min: -1.57079632679f max:-3.1415
            PushMatrix(*model);
                *model = *model * Matrix_Scale(0.2f, 0.6f, 0.2f);
                glUseProgram(shader.program_id);
                // glUniform1i(shader.texture0_uniform         , shader.man_texture.texture_unit);
                glUniformMatrix4fv(shader.view_uniform      , 1 , GL_FALSE , glm::value_ptr(*view));
                glUniformMatrix4fv(shader.projection_uniform, 1 , GL_FALSE , glm::value_ptr(*projection));
                glUniformMatrix4fv(shader.model_uniform     , 1 , GL_FALSE , glm::value_ptr(*model));
                glUniform1i(shader.n_lights_uniform         , lighting->n_lights);
                glUniform4fv(shader.light_positions_uniform , lighting->n_lights , glm::value_ptr(lighting->positions[0]));
                glUniform3fv(shader.light_colors_uniform    , lighting->n_lights , glm::value_ptr(lighting->colors[0]));
                DrawVirtualObject(model_name.c_str(), shader.program_id);
            PopMatrix(*model);
            PushMatrix(*model);
                // antebraço
                *model = *model * Matrix_Translate(0.0f, MinMax(sinusoidal_time_passed, -0.85f, -1.25f), MinMax(sinusoidal_time_passed, 0.4f, 0.0f)); //  min: x, -0.85f, 0.4f max: (0.0f, -1.25f, 0.0f)
                *model = *model
                        * Matrix_Rotate_Z(-0.0f)
                        * Matrix_Rotate_X(MinMax(sinusoidal_time_passed, -1.57079632679f, 0.0f));// min: -1.57079632679f, max: 0.0f
                // *model = *model * Matrix_Translate(0.0f, -1.25f, 0.0f);
                // *model = *model
                //         * Matrix_Rotate_Z(0.0f)
                //         * Matrix_Rotate_X(0.0f);
                PushMatrix(*model);
                    *model = *model * Matrix_Scale(0.2f, 0.6f, 0.2f);
                    glUseProgram(shader.program_id);
                    // glUniform1i(shader.texture0_uniform         , shader.man_texture.texture_unit);
                    glUniformMatrix4fv(shader.view_uniform      , 1 , GL_FALSE , glm::value_ptr(*view));
                    glUniformMatrix4fv(shader.projection_uniform, 1 , GL_FALSE , glm::value_ptr(*projection));
                    glUniformMatrix4fv(shader.model_uniform     , 1 , GL_FALSE , glm::value_ptr(*model));
                    glUniform1i(shader.n_lights_uniform         , lighting->n_lights);
                    glUniform4fv(shader.light_positions_uniform , lighting->n_lights , glm::value_ptr(lighting->positions[0]));
                    glUniform3fv(shader.light_colors_uniform    , lighting->n_lights , glm::value_ptr(lighting->colors[0]));
                    DrawVirtualObject(model_name.c_str(), shader.program_id);
                PopMatrix(*model);

                PushMatrix(*model);
                    *model = *model * Matrix_Translate(0.0f, -0.80f, 0.0f) * Matrix_Scale(0.2f, 0.15f, 0.2f);
                    glUseProgram(shader.program_id);
                    // glUniform1i(shader.texture0_uniform         , shader.man_texture.texture_unit);
                    glUniformMatrix4fv(shader.view_uniform      , 1 , GL_FALSE , glm::value_ptr(*view));
                    glUniformMatrix4fv(shader.projection_uniform, 1 , GL_FALSE , glm::value_ptr(*projection));
                    glUniformMatrix4fv(shader.model_uniform     , 1 , GL_FALSE , glm::value_ptr(*model));
                    glUniform1i(shader.n_lights_uniform         , lighting->n_lights);
                    glUniform4fv(shader.light_positions_uniform , lighting->n_lights , glm::value_ptr(lighting->positions[0]));
                    glUniform3fv(shader.light_colors_uniform    , lighting->n_lights , glm::value_ptr(lighting->colors[0]));
                    DrawVirtualObject(model_name.c_str(), shader.program_id);
                PopMatrix(*model);
            PopMatrix(*model);
        PopMatrix(*model);
    PopMatrix(*model);

    PushMatrix(*model);
        *model = *model * Matrix_Translate(1.10f, MinMax(sinusoidal_time_passed, 0.7f, 1.05f), MinMax(sinusoidal_time_passed, 0.4f, 0.0f)); // min: (x,0.7,0.4f) max (x, 1.05f, 0.0f)
        PushMatrix(*model);
            //braço
            *model = *model
                    * Matrix_Rotate_Z(-0.0f)
                    * Matrix_Rotate_Y(-0.0f)
                    * Matrix_Rotate_X(MinMax(sinusoidal_time_passed, -1.57079632679f, -3.141592f));//min: -1.57079632679f max:-3.1415
            PushMatrix(*model);
                *model = *model * Matrix_Scale(0.2f, 0.6f, 0.2f);
                glUseProgram(shader.program_id);
                // glUniform1i(shader.texture0_uniform         , shader.man_texture.texture_unit);
                glUniformMatrix4fv(shader.view_uniform      , 1 , GL_FALSE , glm::value_ptr(*view));
                glUniformMatrix4fv(shader.projection_uniform, 1 , GL_FALSE , glm::value_ptr(*projection));
                glUniformMatrix4fv(shader.model_uniform     , 1 , GL_FALSE , glm::value_ptr(*model));
                glUniform1i(shader.n_lights_uniform         , lighting->n_lights);
                glUniform4fv(shader.light_positions_uniform , lighting->n_lights , glm::value_ptr(lighting->positions[0]));
                glUniform3fv(shader.light_colors_uniform    , lighting->n_lights , glm::value_ptr(lighting->colors[0]));
                DrawVirtualObject(model_name.c_str(), shader.program_id);
            PopMatrix(*model);
            PushMatrix(*model);
                // antebraço esquerdo
                *model = *model * Matrix_Translate(0.0f, MinMax(sinusoidal_time_passed, -0.85f, -1.25f), MinMax(sinusoidal_time_passed, 0.4f, 0.0f)); //  min: x, -0.85f, 0.4f max: (0.0f, -1.25f, 0.0f)
                *model = *model
                        * Matrix_Rotate_Z(-0.0f)
                        * Matrix_Rotate_X(MinMax(sinusoidal_time_passed, -1.57079632679f, 0.0f));// min: -1.57079632679f, max: 0.0f
                PushMatrix(*model);
                    *model = *model * Matrix_Scale(0.2f, 0.6f, 0.2f);
                    glUseProgram(shader.program_id);
                    // glUniform1i(shader.texture0_uniform         , shader.man_texture.texture_unit);
                    glUniformMatrix4fv(shader.view_uniform      , 1 , GL_FALSE , glm::value_ptr(*view));
                    glUniformMatrix4fv(shader.projection_uniform, 1 , GL_FALSE , glm::value_ptr(*projection));
                    glUniformMatrix4fv(shader.model_uniform     , 1 , GL_FALSE , glm::value_ptr(*model));
                    glUniform1i(shader.n_lights_uniform         , lighting->n_lights);
                    glUniform4fv(shader.light_positions_uniform , lighting->n_lights , glm::value_ptr(lighting->positions[0]));
                    glUniform3fv(shader.light_colors_uniform    , lighting->n_lights , glm::value_ptr(lighting->colors[0]));
                    DrawVirtualObject(model_name.c_str(), shader.program_id);
                PopMatrix(*model);

                PushMatrix(*model);
                    *model = *model * Matrix_Translate(0.0f, -0.80f, 0.0f) * Matrix_Scale(0.2f, 0.15f, 0.2f);
                    glUseProgram(shader.program_id);
                    // glUniform1i(shader.texture0_uniform         , shader.man_texture.texture_unit);
                    glUniformMatrix4fv(shader.view_uniform      , 1 , GL_FALSE , glm::value_ptr(*view));
                    glUniformMatrix4fv(shader.projection_uniform, 1 , GL_FALSE , glm::value_ptr(*projection));
                    glUniformMatrix4fv(shader.model_uniform     , 1 , GL_FALSE , glm::value_ptr(*model));
                    glUniform1i(shader.n_lights_uniform         , lighting->n_lights);
                    glUniform4fv(shader.light_positions_uniform , lighting->n_lights , glm::value_ptr(lighting->positions[0]));
                    glUniform3fv(shader.light_colors_uniform    , lighting->n_lights , glm::value_ptr(lighting->colors[0]));
                    DrawVirtualObject(model_name.c_str(), shader.program_id);
                PopMatrix(*model);
            PopMatrix(*model);
        PopMatrix(*model);
    PopMatrix(*model);

    //pernas
    PushMatrix(*model);
        *model = *model * Matrix_Translate(-0.35f, -1.25f, 0.6f);
        PushMatrix(*model);
            PushMatrix(*model);
                *model = *model * Matrix_Rotate_X(1.57079632679) * Matrix_Scale(-0.325f, 0.8f, 0.2f);
                glUseProgram(shader.program_id);
                // glUniform1i(shader.texture0_uniform         , shader.man_texture.texture_unit);
                glUniformMatrix4fv(shader.view_uniform      , 1 , GL_FALSE , glm::value_ptr(*view));
                glUniformMatrix4fv(shader.projection_uniform, 1 , GL_FALSE , glm::value_ptr(*projection));
                glUniformMatrix4fv(shader.model_uniform     , 1 , GL_FALSE , glm::value_ptr(*model));
                glUniform1i(shader.n_lights_uniform         , lighting->n_lights);
                glUniform4fv(shader.light_positions_uniform , lighting->n_lights , glm::value_ptr(lighting->positions[0]));
                glUniform3fv(shader.light_colors_uniform    , lighting->n_lights , glm::value_ptr(lighting->colors[0]));
                DrawVirtualObject(model_name.c_str(), shader.program_id);
            PopMatrix(*model);
            PushMatrix(*model);
                *model = *model * Matrix_Translate(0.0f, -1.05f, 0.6f);
                PushMatrix(*model);
                    *model = *model * Matrix_Scale(0.25f, 0.8f, 0.2f);
                    glUseProgram(shader.program_id);
                    // glUniform1i(shader.texture0_uniform         , shader.man_texture.texture_unit);
                    glUniformMatrix4fv(shader.view_uniform      , 1 , GL_FALSE , glm::value_ptr(*view));
                    glUniformMatrix4fv(shader.projection_uniform, 1 , GL_FALSE , glm::value_ptr(*projection));
                    glUniformMatrix4fv(shader.model_uniform     , 1 , GL_FALSE , glm::value_ptr(*model));
                    glUniform1i(shader.n_lights_uniform         , lighting->n_lights);
                    glUniform4fv(shader.light_positions_uniform , lighting->n_lights , glm::value_ptr(lighting->positions[0]));
                    glUniform3fv(shader.light_colors_uniform    , lighting->n_lights , glm::value_ptr(lighting->colors[0]));
                    DrawVirtualObject(model_name.c_str(), shader.program_id);
                PopMatrix(*model);

                PushMatrix(*model);
                    *model = *model * Matrix_Translate(0.0f, -1.0f, 0.1f) * Matrix_Scale(0.2f, 0.15f, 0.4f);
                    glUseProgram(shader.program_id);
                    // glUniform1i(shader.texture0_uniform         , shader.man_texture.texture_unit);
                    glUniformMatrix4fv(shader.view_uniform      , 1 , GL_FALSE , glm::value_ptr(*view));
                    glUniformMatrix4fv(shader.projection_uniform, 1 , GL_FALSE , glm::value_ptr(*projection));
                    glUniformMatrix4fv(shader.model_uniform     , 1 , GL_FALSE , glm::value_ptr(*model));
                    glUniform1i(shader.n_lights_uniform         , lighting->n_lights);
                    glUniform4fv(shader.light_positions_uniform , lighting->n_lights , glm::value_ptr(lighting->positions[0]));
                    glUniform3fv(shader.light_colors_uniform    , lighting->n_lights , glm::value_ptr(lighting->colors[0]));
                    DrawVirtualObject(model_name.c_str(), shader.program_id);
                PopMatrix(*model);
            PopMatrix(*model);
        PopMatrix(*model);
    PopMatrix(*model);

    PushMatrix(*model);
        *model = *model * Matrix_Translate(0.35f, -1.25f, 0.6f);
        PushMatrix(*model);
            PushMatrix(*model);
                *model = *model * Matrix_Rotate_X(1.57079632679) * Matrix_Scale(0.325f, 0.8f, 0.2f);
                glUseProgram(shader.program_id);
                // glUniform1i(shader.texture0_uniform         , shader.man_texture.texture_unit);
                glUniformMatrix4fv(shader.view_uniform      , 1 , GL_FALSE , glm::value_ptr(*view));
                glUniformMatrix4fv(shader.projection_uniform, 1 , GL_FALSE , glm::value_ptr(*projection));
                glUniformMatrix4fv(shader.model_uniform     , 1 , GL_FALSE , glm::value_ptr(*model));
                glUniform1i(shader.n_lights_uniform         , lighting->n_lights);
                glUniform4fv(shader.light_positions_uniform , lighting->n_lights , glm::value_ptr(lighting->positions[0]));
                glUniform3fv(shader.light_colors_uniform    , lighting->n_lights , glm::value_ptr(lighting->colors[0]));
                DrawVirtualObject(model_name.c_str(), shader.program_id);
            PopMatrix(*model);

            PushMatrix(*model);
                *model = *model * Matrix_Translate(0.0f, -1.05f, 0.6f);
                PushMatrix(*model);
                    *model = *model * Matrix_Scale(0.25f, 0.8f, 0.2f);
                    glUseProgram(shader.program_id);
                    // glUniform1i(shader.texture0_uniform         , shader.man_texture.texture_unit);
                    glUniformMatrix4fv(shader.view_uniform      , 1 , GL_FALSE , glm::value_ptr(*view));
                    glUniformMatrix4fv(shader.projection_uniform, 1 , GL_FALSE , glm::value_ptr(*projection));
                    glUniformMatrix4fv(shader.model_uniform     , 1 , GL_FALSE , glm::value_ptr(*model));
                    glUniform1i(shader.n_lights_uniform         , lighting->n_lights);
                    glUniform4fv(shader.light_positions_uniform , lighting->n_lights , glm::value_ptr(lighting->positions[0]));
                    glUniform3fv(shader.light_colors_uniform    , lighting->n_lights , glm::value_ptr(lighting->colors[0]));
                    DrawVirtualObject(model_name.c_str(), shader.program_id);
                PopMatrix(*model);

                PushMatrix(*model);
                    *model = *model * Matrix_Translate(0.0f, -1.0f, 0.1f) * Matrix_Scale(0.2f, 0.15f, 0.4f);
                    glUseProgram(shader.program_id);
                    // glUniform1i(shader.texture0_uniform         , shader.man_texture.texture_unit);
                    glUniformMatrix4fv(shader.view_uniform      , 1 , GL_FALSE , glm::value_ptr(*view));
                    glUniformMatrix4fv(shader.projection_uniform, 1 , GL_FALSE , glm::value_ptr(*projection));
                    glUniformMatrix4fv(shader.model_uniform     , 1 , GL_FALSE , glm::value_ptr(*model));
                    glUniform1i(shader.n_lights_uniform         , lighting->n_lights);
                    glUniform4fv(shader.light_positions_uniform , lighting->n_lights , glm::value_ptr(lighting->positions[0]));
                    glUniform3fv(shader.light_colors_uniform    , lighting->n_lights , glm::value_ptr(lighting->colors[0]));
                    DrawVirtualObject(model_name.c_str(), shader.program_id);
                PopMatrix(*model);
            PopMatrix(*model);
        PopMatrix(*model);
    PopMatrix(*model);
}

void Man::Destroy() {

}

float Man::MinMax(float x, float min, float max) {
    return x * (max - min) + min;
    // return (x - min) / max - min;
}

glm::mat4 Man::ComputeTransform() {
    return Matrix_Identity() 
        * Matrix_Translate(position.x, position.y, position.z)
        * Matrix_Rotate_Z(0.0)
        * Matrix_Rotate_Y(0.0)
        * Matrix_Rotate_X(0.0)
        * Matrix_Scale(scale, scale, scale);
}

SphereShape Man::GetSphereShape()
{
    SphereShape s;
    return s;
}

OBBShape Man::GetOBBShape()
{
    OBBShape s;
    return s;
}

PlaneShape Man::GetPlaneShape()
{
    PlaneShape s;
    return s;
}
