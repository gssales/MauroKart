#include "input.h"

void Input::Update()
{
    std::map<int, KeyState>::iterator it = key_states.begin();

    while (it != key_states.end()) {
        KeyState state = it->second;

        state.is_pressed = false;
        state.is_released = false;

        it->second = state;

        it++;
    }

    cursor_changed = false;
    scroll_changed = false;
}

void Input::KeyCallback(int key, int action, int mod)
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

KeyState Input::GetKeyState(int key)
{
    KeyState key_state;

    if (key_states.count(key) > 0)
        key_state = key_states[key];

    return key_state;
}
