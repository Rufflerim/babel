#include "pch.h"
#include "Engine.h"

void engine::Engine::init(IGame& game)
{
	state.game = &game;

	// Init everything

	state.isInitialized = true;
	if (!state.isInitialized) {
		LOG(LogLevel::Fatal) << "Engine subsystems failed at init. Shutting down.";
		// Shut down all systems
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

		// Update
		update(time);

		// Draw
		draw();

		// Time delay if game loop is faster than target FPS
		timer.delayTime();

		state.isRunning = false;
	}
}

void engine::Engine::close()
{
	LOG(LogLevel::Info) << "Engine closed. Bye :)";
}

void engine::Engine::update(GameTime time)
{
	state.game->update(time);
}

void engine::Engine::draw()
{
	state.game->draw();
}
