#include "gamestate.h"

Input input;

float g_ScreenRatio = (float)1280 / 720;

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

GameObject::GameObject()
{
    creation_time = glfwGetTime()*1000;
}


