#ifndef _MODEL_H
#define _MODEL_H

#include <stdlib.h>
#include <tiny_obj_loader.h>
#include <glm/vec4.hpp>
#include "matrices.h"

class ObjModel
{
public:
    tinyobj::attrib_t                 attrib;
    std::vector<tinyobj::shape_t>     shapes;
    std::vector<tinyobj::material_t>  materials;
    
    ObjModel(const char* filename, const char* basepath = NULL, bool triangulate = true);
    void ComputeNormals();
};

#endif // _MODEL_H
