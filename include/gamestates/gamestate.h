#ifndef _GAMESTATE_H
#define _GAMESTATE_H

#include <ctime>
#include <map>
#include <vector>
#include <string>
#include "gl.h"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "matrices.h"
#include "input.h"
#include "scene.h"
#include "graphics/light.h"
#include "graphics/gpu_program.h"
#include "physics/shapes.h"

extern Input input;
extern float g_ScreenRatio;

// Pilha que guardará as matrizes de modelagem.
extern std::stack<glm::mat4>  g_MatrixStack;

// Função que pega a matriz M e guarda a mesma no topo da pilha
void PushMatrix(glm::mat4 M);

// Função que remove a matriz atualmente no topo da pilha e armazena a mesma na variável M
void PopMatrix(glm::mat4& M);

class LightSet
{
public:
    glm::vec4 positions[128];
    glm::vec3 colors[128];
    int n_lights;
};

class GameState;

class GameObject
{
public:
    ShapeType shape_type;
    int creation_time;
    glm::vec4 position;
    bool dead = false;

    GameObject();
    virtual void Update(double dt) = 0;
    virtual void Render(glm::mat4* model, glm::mat4* view, glm::mat4* projection, GpuProgram* default_shader, LightSet* lighting) = 0;
    virtual void Destroy() = 0;
    virtual glm::mat4 ComputeTransform() = 0;
    virtual SphereShape GetSphereShape() = 0;
    virtual OBBShape GetOBBShape() = 0;
    virtual PlaneShape GetPlaneShape() = 0;
};

class GameState
{
public:
    std::map<std::string, GameObject*> game_objects;

    void Load();
    void Update(double dt);
    void Render();
};


#endif // _GAMESTATE_H
