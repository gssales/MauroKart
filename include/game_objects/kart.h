#ifndef _KART_H
#define _KART_H

#include <string>
#include "gamestates/gamestate.h"
#include "graphics/camera.h"

class KartShader: public GpuProgram
{
public:
    Texture kart_texture0;
    GLint n_lights_uniform;
    GLint light_positions_uniform;
    GLint light_colors_uniform;
    GLint texture0_uniform;

    KartShader() {};
    KartShader(bool build);
};

class WheelShader: public GpuProgram
{
public:
    Texture wheel_texture0;
    GLint n_lights_uniform;
    GLint light_positions_uniform;
    GLint light_colors_uniform;
    GLint texture0_uniform;

    WheelShader() {};
    WheelShader(bool build);
};

class Kart: public GameObject
{
public:
    KartShader shader;
    WheelShader wheel_shader;
    Camera camera;
    std::string model_name;
    glm::vec4 movement_vec;
    bool touch_ground = false;
    float speed;
    float acceleration;
    float max_speed;
    float rx = 0.0f;
    float ry = 0.0f;
    float rz = 0.0f;
    float turning = 0.0;

    double cooldown_camera = 0.0;

    Kart();
    void Update(double dt);
    void Render(glm::mat4* model, glm::mat4* view, glm::mat4* projection, GpuProgram* default_shader, LightSet* lighting);
    void Destroy();
    glm::mat4 ComputeTransform();
    SphereShape GetSphereShape();
    OBBShape GetOBBShape();
    PlaneShape GetPlaneShape();
};

#endif // _KART_H
