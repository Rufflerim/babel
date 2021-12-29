//
// Created by gaetz on 12/06/2021.
//

#ifndef INPUT_KEYBOARDSTATE_H
#define INPUT_KEYBOARDSTATE_H

#include <SDL_keyboard.h>
#include "../Defines.h"


namespace engine { namespace input {

    enum class ButtonState;


    class KeyboardState {
        friend class InputManager;

    public:
        bool getKeyValue(SDL_Scancode key) const;
        ButtonState getKeyState(SDL_Scancode key) const;

    private:
        const u8* currentState;
        Uint8 previousState[SDL_NUM_SCANCODES];
    };
}}

#endif //INPUT_KEYBOARDSTATE_H
