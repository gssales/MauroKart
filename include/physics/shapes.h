#ifndef _SHAPES_H
#define _SHAPES_H

#include <cstdio>
#include <glm/vec4.hpp>

#include "matrices.h"

enum ShapeType: int
{
    OBB_SHAPE,
    SPHERE_SHAPE,
    PLANE_SHAPE,
};

class Shape { };

class PlaneShape: Shape
{
public:
    glm::vec4 point = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 normal = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    glm::vec4 limits[6] = {
        glm::vec4( 1.0f, 0.0f, 1.0f, 1.0f),
        glm::vec4( 1.0f, 0.0f,-1.0f, 1.0f),
        glm::vec4(-1.0f, 0.0f,-1.0f, 1.0f),
        glm::vec4(-1.0f, 0.0f, 1.0f, 1.0f)
    };
};

class SphereShape: Shape
{
public:
    glm::vec4 point = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    float radius = 1.0f;
};

struct Axis
{
    glm::vec4 x = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec4 y = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    glm::vec4 z = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
};

struct Rotation
{
    float alpha = 0.0;
    float beta = 0.0;
    float gamma = 0.0;
};

class OBBShape: Shape
{
public:
    glm::vec4 center = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    Axis axis;
    Rotation rotation;
    glm::vec4 half_length = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
};

class AABBShape: Shape
{
public:
    glm::vec4 bb_min;
    glm::vec4 bb_max;
};

void Print_OBBShape(OBBShape s);

void Print_SphereShape(SphereShape s);

void Print_PlaneShape(PlaneShape s);

#endif // _SHAPES_H
