#include "InputManager.h"
#include "../Log.h"

using engine::input::InputManager;

bool InputManager::init() {
	LOG(LogLevel::Trace) << "Inputs initialized";
    return true;
}

void InputManager::close() {
}

void InputManager::processSDLEvent(SDL_Event& event, bool& engineIsRunning) {
	switch (event.type)
	{
	case SDL_QUIT:
		engineIsRunning = false;
		break;

	default:
		break;
	}
}
