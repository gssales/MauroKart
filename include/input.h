#ifndef _INPUT_H
#define _INPUT_H

#include <map>
#include <GLFW/glfw3.h>  // Criação de janelas do sistema operacional

struct KeyState
{
    int key = -1;
    bool is_pressed = false;
    bool is_released = false;
    bool is_down = false;
    int modifiers = 0;
};

class Input
{
public:
    std::map<int, KeyState> key_states;

    void Update();
    void KeyCallback(int key, int action, int mod);
    KeyState GetKeyState(int key);
};

#endif // INPUT_H_INCLUDED
