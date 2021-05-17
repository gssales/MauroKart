#ifndef _BUNNY_H
#define _BUNNY_H

#include <string>
#include <algorithm>
#include "gamestates/gamestate.h"
#include "graphics/camera.h"

class BunnyShader: public GpuProgram
{
public:
    Texture bunny_texture;
    GLint n_lights_uniform;
    GLint light_positions_uniform;
    GLint light_colors_uniform;
    GLint texture0_uniform;

    BunnyShader() {};
    BunnyShader(const char* vertex_shader_filename, const char* fragment_shader_filename, const char* image_filename);
};

class Bunny: public GameObject
{
public:
    BunnyShader shader;
    Camera camera;
    std::string model_name;
    glm::vec3 movement_vec;
    float speed;
    float acceleration;
    float max_speed;
    float ry = 0.0f;

    // Bunny(GameState* game_state);
    Bunny();
    void Update(double dt);
    void Render(glm::mat4* model, glm::mat4* view, glm::mat4* projection, GpuProgram* default_shader, LightSet* lighting);
    void Destroy();
    glm::mat4 ComputeTransform();
    SphereShape GetSphereShape();
    OBBShape GetOBBShape();
    PlaneShape GetPlaneShape();
};

#endif // _BUNNY_H
