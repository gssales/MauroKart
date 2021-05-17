#include "graphics/camera.h"

#include "matrices.h"

void Camera::Update()
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

        position = lookat + glm::vec4(x,y,z,0.0f);
        view_vector = lookat - position;
    }

}

glm::mat4 Camera::View()
{
    return Matrix_Camera_View(position, view_vector, up_vector);
}

glm::mat4 Camera::Projection(float screen_ratio)
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
