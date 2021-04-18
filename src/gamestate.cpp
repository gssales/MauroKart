#include "gamestate.h"

Input input;

float g_ScreenRatio = 1.0f;

std::stack<glm::mat4>  g_MatrixStack;

void PushMatrix(glm::mat4 M)
{
    g_MatrixStack.push(M);
}

// Função que remove a matriz atualmente no topo da pilha e armazena a mesma na variável M
void PopMatrix(glm::mat4& M)
{
    if ( g_MatrixStack.empty() )
    {
        M = Matrix_Identity();
    }
    else
    {
        M = g_MatrixStack.top();
        g_MatrixStack.pop();
    }
}

// GameObject::GameObject(GameState *game_state) {
//     this->game_state = game_state;
// }


