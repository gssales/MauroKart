#ifndef _FLOOR_H
#define _FLOOR_H

#include <string>
#include <algorithm>
#include "gamestates/gamestate.h"
#include "graphics/camera.h"

class TrackShader: public GpuProgram
{
public:
    Texture track_texture;
    GLint n_lights_uniform;
    GLint light_positions_uniform;
    GLint light_colors_uniform;
    GLint texture0_uniform;

    TrackShader() {};
    TrackShader(bool build);
};

class Track: public GameObject
{
public:
    TrackShader shader;
    Camera camera;
    std::string model_name;
    float scale = 1.0f;
    float time_passed;

    Track();
    void Update(double dt);
    void Render(glm::mat4* model, glm::mat4* view, glm::mat4* projection, GpuProgram* default_shader, LightSet* lighting);
    void Destroy();
    glm::mat4 ComputeTransform();
    SphereShape GetSphereShape();
    OBBShape GetOBBShape();
    PlaneShape GetPlaneShape();
};

#endif // _FLOOR_H
