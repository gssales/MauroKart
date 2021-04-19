#ifndef _INPUT_H
#define _INPUT_H

#include <map>
#include "gl.h" // Criação de janelas do sistema operacional

struct KeyState
{
    int key = -1;
    bool is_pressed = false;
    bool is_released = false;
    bool is_down = false;
    int modifiers = 0;
};

struct PairState
{
    double xvalue = 0.0;
    double yvalue = 0.0;
};

class Input
{
public:
    std::map<int, KeyState> key_states;
    PairState scroll_state;
    PairState cursor_state;
    bool scroll_changed = false;
    bool cursor_changed = false;

    void Update();
    void KeyCallback(int key, int action, int mod);
    KeyState GetKeyState(int key);
};

#endif // INPUT_H_INCLUDED
