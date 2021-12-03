#include "Engine.h"
#include "Log.h"
#include "Timer.h"
#include <SDL_events.h>


using engine::InputState;

void engine::Engine::init(IGame& game)
{
	state.game = &game;

	// Init everything
	bool inputsIgnited = inputManager.init();
	bool windowIgnited = window.init(WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, false);



	state.isInitialized = inputsIgnited && windowIgnited;
	if (!state.isInitialized) {
		LOG(LogLevel::Fatal) << "Engine subsystems failed at init. Shutting down.";
		// Shut down all systems
		inputManager.close();
	}
	else {
		state.isRunning = true;
		state.isPaused = false;
		LOG(LogLevel::Info) << "Engine started.";
	}

}

void engine::Engine::run()
{
	// Timer for delta time
	Timer timer;
	GameTime time;

	state.game->load();
	// Loop
	while (state.isRunning) {
		// TODO Compute delta time
		time = timer.computeTime(time);

		// Inputs
		InputState inputState = inputs();

		// Update
		window.updateFPSCounter(time);
		update(time);

		// Draw
		draw();

		// Time delay if game loop is faster than target FPS
		timer.delayTime();
	}
}

void engine::Engine::close()
{
	window.close();
	inputManager.close();
	LOG(LogLevel::Info) << "Engine closed.";
	LOG(LogLevel::Trace) << "Bye :)";
}

InputState engine::Engine::inputs()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		inputManager.processSDLEvent(event, state.isRunning);
	}

	return inputManager.getInputState();
}

void engine::Engine::update(GameTime time)
{
	state.game->update(time);
}

void engine::Engine::draw()
{
	state.game->draw();
}
