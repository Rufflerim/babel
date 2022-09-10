#include <Functions.h>
#include "InputManager.h"

using engine::input::InputManager;

InputManager::InputManager(u32 windowWidthP, u32 windowHeightP) :
        windowWidth {windowWidthP},
        windowHeight {windowHeightP} {

}

bool InputManager::init(engine::ILocator* locatorP) {
    locator = locatorP;
    // Keyboard
    // Assign current state pointer
    inputState.keyboard.currentState = SDL_GetKeyboardState(nullptr);
    // Clear previous state memory
    memset(inputState.keyboard.previousState, 0, SDL_NUM_SCANCODES);

    // Mouse (just set everything to 0)
    inputState.mouse.currentButtons = 0;
    inputState.mouse.previousButtons = 0;

    // Get the connected controllerPtr, if it exists
    controllerPtr = SDL_GameControllerOpen(0);
    // Initialize controllerPtr state
    inputState.controller.isConnected = (controllerPtr != nullptr);
    i16 maxControllerButton = static_cast<i16>(ControllerButton::Max);
    memset(inputState.controller.currentButtons, 0, maxControllerButton);
    memset(inputState.controller.previousButtons, 0, maxControllerButton);

    LOG(LogLevel::Trace) << "Inputs initialized";
    return true;
}

void InputManager::close() {
    if (controllerPtr != nullptr) {
        SDL_GameControllerClose(controllerPtr);
    }
}

void InputManager::processSDLEvent(SDL_Event& event) {
    switch (event.type) {
        case SDL_QUIT:
            locator->events().fire(EventCode::ApplicationQuit, nullptr);
            break;
        case SDL_MOUSEWHEEL:
            inputState.mouse.scrollWheel = Vec2 {
                    static_cast<float>(event.wheel.x),
                    static_cast<float>(event.wheel.y)
            };
            break;
        default:
            break;
    }
}

void InputManager::preUpdate() {
    // Copy current state to previous
    // Keyboard
    memcpy(inputState.keyboard.previousState, inputState.keyboard.currentState, SDL_NUM_SCANCODES);
    // Mouse
    inputState.mouse.previousButtons = inputState.mouse.currentButtons;
    inputState.mouse.scrollWheel = Vec2(0, 0);
    // Controller
    i16 maxControllerButton = static_cast<i16>(ControllerButton::Max);
    memcpy(inputState.controller.previousButtons, inputState.controller.currentButtons, maxControllerButton);
}

void InputManager::update() {
    // Mouse
    i32 mouseX = 0, mouseY = 0;
    inputState.mouse.currentButtons = inputState.mouse.isRelativeMode ? SDL_GetRelativeMouseState(&mouseX, &mouseY)
                                                                      : SDL_GetMouseState(&mouseX, &mouseY);
    inputState.mouse.position.x = static_cast<float>(mouseX);
    inputState.mouse.position.y = static_cast<float>(mouseY);
    if (!inputState.mouse.isRelativeMode) {
        // We want mouse coordinates to be between minus half the screen's size and plus half the screen size
        inputState.mouse.position.x -= static_cast<float>(windowWidth) * 0.5f;
        inputState.mouse.position.y = static_cast<float>(windowHeight) * 0.5f - inputState.mouse.position.y;
    }

    // Controller
    // Buttons
    i16 controllerMaxButton = static_cast<i16>(ControllerButton::Max);
    for (i32 i = 0; i < controllerMaxButton; i++) {
        inputState.controller.currentButtons[i] = SDL_GameControllerGetButton(controllerPtr, SDL_GameControllerButton(i));
    }

    // Triggers
    inputState.controller.leftTrigger = filter1D(
            SDL_GameControllerGetAxis(controllerPtr, static_cast<SDL_GameControllerAxis>(ControllerAxis::TriggerLeft)));
    inputState.controller.rightTrigger = filter1D(
            SDL_GameControllerGetAxis(controllerPtr, static_cast<SDL_GameControllerAxis>(ControllerAxis::TriggerRight)));

    // Sticks
    i32 leftStickX = SDL_GameControllerGetAxis(controllerPtr, static_cast<SDL_GameControllerAxis>(ControllerAxis::LeftX));
    i32 leftStickY = -SDL_GameControllerGetAxis(controllerPtr, static_cast<SDL_GameControllerAxis>(ControllerAxis::LeftY));
    inputState.controller.leftStick = filter2D(leftStickX, leftStickY);

    i32 rightStickX = SDL_GameControllerGetAxis(controllerPtr, static_cast<SDL_GameControllerAxis>(ControllerAxis::RightX));
    i32 rightStickY = -SDL_GameControllerGetAxis(controllerPtr, static_cast<SDL_GameControllerAxis>(ControllerAxis::RightY));
    inputState.controller.rightStick = filter2D(rightStickX, rightStickY);
}

void InputManager::setMouseCursor(bool isCursorDisplayedP) {
    isCursorDisplayed = isCursorDisplayedP;
    SDL_ShowCursor(isCursorDisplayed ? SDL_TRUE : SDL_FALSE);
}

void InputManager::setMouseRelativeMode(bool isMouseRelativeOnP) {
    inputState.mouse.isRelativeMode = isMouseRelativeOnP;
    SDL_SetRelativeMouseMode(isMouseRelativeOnP ? SDL_TRUE : SDL_FALSE);
}

float InputManager::filter1D(i32 input) {
    const i32 deadZone = CONTROLLER_DEAD_ZONE_1D;
    const i32 maxValue = CONTROLLER_MAX_VALUE;
    f32 retVal = 0.0f;

    i32 absValue = input > 0 ? input : -input;
    if (absValue > deadZone) {
        // Compute fractional value between dead zone and max value
        retVal = static_cast<float>(absValue - deadZone) / (maxValue - deadZone);
        // Make sure sign matches original value
        retVal = input > 0 ? retVal : -1.0f * retVal;
        // Clamp between -1.0f and 1.0f
        retVal = gmath::clamp(retVal, -1.0f, 1.0f);
    }

    return retVal;
}

Vec2 InputManager::filter2D(i32 inputX, i32 inputY) {
    const float deadZone = CONTROLLER_DEAD_ZONE_2D;
    const float maxValue = CONTROLLER_MAX_VALUE;

    Vec2 dir;
    dir.x = static_cast<f32>(inputX);
    dir.y = static_cast<f32>(inputY);
    auto length = static_cast<f32>(dir.length());

    // If length < deadZone, should be no input
    if (length < deadZone) {
        dir = Vec2(0, 0);
    } else {
        // Calculate fractional value between dead zone and max value circles
        float f = (length - deadZone) / (maxValue - deadZone);
        // Clamp f between 0.0f and 1.0f
        f = gmath::clamp(f, 0.0f, 1.0f);
        // Normalize the vector, and then scale it to the fractional value
        dir *= f / length;
    }

    return dir;
}
