#ifndef _COLLISIONS_H
#define _COLLISIONS_H

// #include <cstdlib>
#include <algorithm>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "physics/shapes.h"
#include "matrices.h"

struct Contact
{
    glm::vec4 contact_point;
    glm::vec4 normal;
    glm::vec4 min_move;
};

void Print_Contact(Contact c);

bool Collide_OBB_Sphere(Contact result[2], OBBShape obb, SphereShape sphere);
bool Collide_OBB_Plane(Contact result[2], OBBShape obb, PlaneShape plane);
bool Collide_OBB_OBB(Contact result[2], OBBShape obb1, OBBShape obb2);

float Ray_Cast_Plane(glm::vec4 ray_origin, glm::vec4 ray_vector, glm::vec4 plane_origin, glm::vec4 plane_normal);

#endif // _COLLISIONS_H