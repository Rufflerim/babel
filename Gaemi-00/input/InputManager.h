#pragma once
#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SDL.h>
#include "../Defines.h"
#include "KeyboardState.h"
#include "MouseState.h"
#include "ControllerState.h"
#include "../math/Vec2.h"
#include "../ILocator.h"

using gmath::Vec2;

namespace engine { namespace input {

    enum class ButtonState {
        None,
        Pressed,
        Released,
        Held
    };

    struct InputState {
        KeyboardState keyboard;
        MouseState mouse;
        ControllerState controller;
    };

    class InputManager {
    public:
        InputManager(u32 windowWidthP, u32 windowHeightP);
        InputManager() = delete;

        bool init(engine::ILocator* locatorP);

        void close();

        const InputState& getInputState() const { return inputState; }

        // Returns isRunning status
        void processSDLEvent(SDL_Event& event);

        void preUpdate();

        void update();

        bool getIsCursorDisplayed() const { return isCursorDisplayed; }

        void setMouseCursor(bool isCursorDisplayedP);

        void setMouseRelativeMode(bool isMouseRelativeOnP);

    private:
        ILocator* locator{nullptr};
        u32 windowWidth { 1280 };
        u32 windowHeight { 720 };
        InputState inputState {};
        bool isCursorDisplayed { false };

#ifndef GPLATFORM_WEB
        SDL_GameController* controllerPtr { nullptr };
#endif
        f32 filter1D(int input);
        Vec2 filter2D(int inputX, int inputY);
    };

    constexpr i32 CONTROLLER_DEAD_ZONE_1D = 250;
    constexpr f32 CONTROLLER_DEAD_ZONE_2D = 8000.0f;
    constexpr i32 CONTROLLER_MAX_VALUE = 30000;
}}

#endif
