#ifndef _LIGHT_H
#define _LIGHT_H

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

class Light
{
public:
    glm::vec4 position = glm::vec4(0.0f,0.0f,0.0f,1.0f);
    glm::vec3 color = glm::vec3(1.0f,1.0f,1.0f);
};

#endif // _LIGHT_H