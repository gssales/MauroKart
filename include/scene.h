#ifndef _SCENE_H
#define _SCENE_H

#include <cstdlib>
#include <algorithm>
#include <map>
#include <stack>
#include "gl.h"
#include <glm/mat4x4.hpp>

#include "graphics/model.h"

struct SceneObject
{
    std::string  name;        // Nome do objeto
    size_t       first_index; // Índice do primeiro vértice dentro do vetor indices[] definido em BuildTrianglesAndAddToVirtualScene()
    size_t       num_indices; // Número de índices do objeto dentro do vetor indices[] definido em BuildTrianglesAndAddToVirtualScene()
    GLenum       rendering_mode; // Modo de rasterização (GL_TRIANGLES, GL_TRIANGLE_STRIP, etc.)
    GLuint       vertex_array_object_id; // ID do VAO onde estão armazenados os atributos do modelo
    glm::vec3    bounding_box_min; // Axis-Aligned Bounding Box do objeto
    glm::vec3    bounding_box_max;
};

extern std::map<std::string, SceneObject> virtualScene;

void AddModelToScene(ObjModel *model);
void DrawVirtualObject(const char* object_name, GLuint program_id);

void Print_SceneObject(SceneObject o);

#endif // _SCENE_H
