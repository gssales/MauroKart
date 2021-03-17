#ifndef _CAMERA_H
#define _CAMERA_H

#include <glm/vec4.hpp>
#include "matrices.h"

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


    void Update()
    {
        if (free)
        {
            view_vector = Matrix_Rotate_Y(theta) * Matrix_Rotate_X(phi)
                    * glm::vec4(0.0f,0.0f,1.0f,0.0f);
        }
        else
        {
            float r = distance;
            float y = r*sin(phi);
            float z = r*cos(phi)*cos(theta);
            float x = r*cos(phi)*sin(theta);

            position = glm::vec4(x,y,z,1.0f);
            view_vector = lookat - position;
        }

    }

    glm::mat4 View()
    {
        return Matrix_Camera_View(position, view_vector, up_vector);
    }

    glm::mat4 Projection(float screen_ratio)
    {
        glm::mat4 projection;

        if (perspective)
        {
            projection = Matrix_Perspective(field_of_view, screen_ratio,
                                            nearplane, farplane);
        }
        else
        {
            float t = 1.5f*distance/2.5f;
            float b = -t;
            float r = t*screen_ratio;
            float l = -r;
            projection = Matrix_Orthographic(l, r, b, t, nearplane, farplane);
        }

        return projection;
    }
};


#endif // _CAMERA_H
