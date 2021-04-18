#include "game_objects/cube.h"

Cube::Cube() {
    time_t t;
    time(&t);
    creation_time = t;
    model_name = "cube";
    fragment_shader = "../../res/shaders/bunny_fs.glsl";

    position = glm::vec3(0.0,0.0,0.0);
}

void Cube::Update(double dt) {
    // printf("teste\n");
}

void Cube::Render(glm::mat4 model) {

    model = model * Matrix_Translate(position.x, position.y, position.z);
    PushMatrix(model);
        model = model
              * Matrix_Rotate_Z(0.0)
              * Matrix_Rotate_Y(0.0)
              * Matrix_Rotate_X(0.0);
        PushMatrix(model);
            model = model * Matrix_Scale(1.0, 1.0, 1.0);
                    glUniformMatrix4fv(model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
                    LoadGpuProgram(default_vs_filename.c_str(), fragment_shader.c_str());
                    DrawVirtualObject(model_name.c_str());
        PopMatrix(model);
    PopMatrix(model);
}

void Cube::Destroy() {

}
