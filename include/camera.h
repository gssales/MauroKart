#ifndef _CAMERA_H
#define _CAMERA_H

#include <cstdio>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

class Camera
{
public:
    glm::vec4 position = glm::vec4(0.0f,0.0f,0.0f,1.0f);

    glm::vec4 view_vector;
    float theta = 0.0f, phi = 0.0f;

    glm::vec4 up_vector = glm::vec4(0.0f,1.0f,0.0f,0.0f);

    bool free = true;
    glm::vec4 lookat = glm::vec4(0.0f,0.0f,0.0f,1.0f);;
    float distance = 1.0f;

    bool perspective = true;
    float field_of_view = 3.141592 / 3.0f;
    float nearplane = -0.1f, farplane = -10.0f;


    void Update();
    glm::mat4 View();
    glm::mat4 Projection(float screen_ratio);
};


#endif // _CAMERA_H
