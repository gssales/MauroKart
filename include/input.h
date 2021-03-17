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

const KeyState KEY_UNDEFINED;

class Input
{
public:
    std::map<int, KeyState> key_states;

    void Update()
    {
        std::map<int, KeyState>::iterator it = key_states.begin();

        while (it != key_states.end()) {
            KeyState state = it->second;

            state.is_pressed = false;
            state.is_released = false;

            it->second = state;

            it++;
        }
    }

    KeyState GetKeyState(int key)
    {
        KeyState key_state = KEY_UNDEFINED;

        if (key_states.count(key) > 0)
            key_state = key_states[key];

        return key_state;
    }

    void KeyCallback(int key, int action, int mod)
    {
        if (key_states.count(key) == 0) {
            KeyState state;
            state.key = key;
            key_states[key] = state;
        }

        key_states[key].is_pressed = action == GLFW_PRESS;
        key_states[key].is_released = action == GLFW_RELEASE;

        if (action == GLFW_PRESS)
            key_states[key].is_down = true;
        if (action == GLFW_RELEASE)
            key_states[key].is_down = false;

        if (key_states[key].is_down)
            key_states[key].modifiers = mod;
        else
            key_states[key].modifiers = 0;
    }
};

#endif // INPUT_H_INCLUDED
