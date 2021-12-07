#include "InputManager.h"
#include "Log.h"

bool engine::InputManager::init()
{
	LOG(LogLevel::Trace) << "Inputs initialized";
    return true;
}

void engine::InputManager::close()
{
}

void engine::InputManager::processSDLEvent(SDL_Event& event, bool& engineIsRunning)
{
	switch (event.type)
	{
	case SDL_QUIT:
		engineIsRunning = false;
		break;

	default:
		break;
	}
}
