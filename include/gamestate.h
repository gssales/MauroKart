#ifndef _GAMESTATE_H
#define _GAMESTATE_H

#include <ctime>
#include <map>
#include <string>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "matrices.h"
#include "input.h"
#include "gpu_program.h"
#include "scene.h"

extern Input input;
extern float g_ScreenRatio;

// Pilha que guardará as matrizes de modelagem.
extern std::stack<glm::mat4>  g_MatrixStack;

// Função que pega a matriz M e guarda a mesma no topo da pilha
void PushMatrix(glm::mat4 M);

// Função que remove a matriz atualmente no topo da pilha e armazena a mesma na variável M
void PopMatrix(glm::mat4& M);

class GameState;

class GameObject
{
public:
    GameState *game_state;
    int creation_time;
    glm::vec3 position;
    bool dead = false;

    // GameObject(GameState* game_state);
    virtual void Update(double dt) = 0;
    virtual void Render(glm::mat4 model) = 0;
    virtual void Destroy() = 0;
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