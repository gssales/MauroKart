#ifndef _PLANE_H
#define _PLANE_H

#include <algorithm>
#include <string>
#include "gamestates/gamestate.h"
#include "game_objects/cube.h"

class Plane: public GameObject
{
public:
    CubeShader shader;
    std::string model_name;
    std::string fragment_shader;
    float rx = 0.0f;
    float ry = 0.0f;
    float rz = 0.0f;
    float s = 1.0f;

    Plane();
    void Update(double dt);
    void Render(glm::mat4* model, glm::mat4* view, glm::mat4* projection, GpuProgram* default_shader, LightSet* lighting);
    void Destroy();
    glm::mat4 ComputeTransform();
    SphereShape GetSphereShape();
    OBBShape GetOBBShape();
    PlaneShape GetPlaneShape();
};

#endif // _PLANE_H
