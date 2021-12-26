//
// Created by gaetz on 12/06/2021.
//

#ifndef INPUT_MOUSESTATE_H
#define INPUT_MOUSESTATE_H

#include "../math/Vec2.h"
#include "../Defines.h"

using gmath::Vec2;

namespace engine::input {
    enum class ButtonState;

    class MouseState {
        friend class InputManager;

    public:
        const Vec2 &getPosition() const { return position; }

        bool getButtonValue(i32 button) const;

        ButtonState getButtonState(i32 button) const;

        const Vec2 &getScrollWheel() const { return scrollWheel; }

        bool isRelativeModeOn() const { return isRelativeMode; }

    private:
        Vec2 position;
        u32 currentButtons;
        u32 previousButtons;
        Vec2 scrollWheel;
        bool isRelativeMode;
    };
}
#endif //INPUT_MOUSESTATE_H
