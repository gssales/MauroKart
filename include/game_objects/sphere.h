#ifndef _SPHERE_H
#define _SPHERE_H

#include <string>
#include "gamestates/gamestate.h"
#include "game_objects/cube.h"

class Sphere: public GameObject
{
public:
    CubeShader shader;
    std::string model_name;
    glm::vec3 movement_vec;
    float speed;
    float acceleration;
    float max_speed;

    Sphere();
    void Update(double dt);
    void Render(glm::mat4* model, glm::mat4* view, glm::mat4* projection, GpuProgram* default_shader, LightSet* lighting);
    void Destroy();
    glm::mat4 ComputeTransform();
    SphereShape GetSphereShape();
    OBBShape GetOBBShape();
    PlaneShape GetPlaneShape();
};

#endif // _SPHERE_H
