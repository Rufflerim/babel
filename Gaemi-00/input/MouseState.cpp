//
// Created by gaetz on 12/06/2021.
//

#include "MouseState.h"
#include <SDL_mouse.h>
#include "InputManager.h"

using engine::input::MouseState;
using engine::input::ButtonState;

bool MouseState::getButtonValue(i32 button) const {
    i32 mask = SDL_BUTTON(button);
    return (mask & currentButtons) == 1;
}

ButtonState MouseState::getButtonState(i32 button) const {
    i32 mask = SDL_BUTTON(button);
    if ((mask & previousButtons) == 0) {
        if ((mask & currentButtons) == 0) {
            return ButtonState::None;
        } else {
            return ButtonState::Pressed;
        }
    } else {
        if ((mask & currentButtons) == 0) {
            return ButtonState::Released;
        } else {
            return ButtonState::Held;
        }
    }
}
