#include "Engine.h"
#include "Log.h"
#include "Timer.h"
#include "../Babel/Locator.h"
#include <SDL_events.h>


using engine::input::InputState;

void engine::Engine::init(IGame* game, ILocator& locator) {
	state.game = game;
    state.locator = &locator;

	// Init everything
	bool inputsIgnited = inputManager.init(&locator);
	bool windowIgnited = window.init(WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, false);
    bool eventsIgnited = eventManager.init();
    eventManager.subscribe(EventCode::ApplicationQuit, nullptr, &onEngineEvent);
    bool rendererIgnited = renderer.init(&locator, window);
    bool assetsIgnited = assetManager.init(renderer);

	state.isInitialized = inputsIgnited && windowIgnited && eventsIgnited && rendererIgnited && assetsIgnited;
	if (!state.isInitialized) {
		LOG(LogLevel::Fatal) << "Engine subsystems failed at init. Shutting down.";
		// Shut down all systems
        close();
	} else {
        // Set state
		state.isRunning = true;
		state.isPaused = false;
        // Setup locator
		locator.init(&eventManager, &assetManager);
        // Announce proudly
        LOG(LogLevel::Info) << "Engine started.";
	}

}

ErrorCode engine::Engine::run() {
	// Timer for delta time
	Timer timer;
	GameTime time;

	state.game->load();
	// Loop
	while (state.isRunning) {
		time = timer.computeTime(time);

		// Inputs
		InputState inputState = inputs();

		// Update
		window.updateFPSCounter(time);
        update(time, inputState);

		// Draw
        renderer.clearScreen();
        renderer.beginDraw();
        draw(renderer);
        renderer.endDraw();

		// Time delay if game loop is faster than target FPS
		timer.delayTime();
	}

    return state.errorCode;
}

void engine::Engine::close() {
    renderer.close();
    eventManager.unsubscribe(EventCode::ApplicationQuit, nullptr, &onEngineEvent);
    eventManager.close();
    assetManager.close();
    window.close();
	inputManager.close();
	LOG(LogLevel::Info) << "Engine closed.";
	LOG(LogLevel::Trace) << "Bye :)";
}

InputState engine::Engine::inputs() {
    inputManager.preUpdate();
    SDL_Event event;
	while (SDL_PollEvent(&event)) {
		inputManager.processSDLEvent(event);
	}
    inputManager.update();

	return inputManager.getInputState();
}

void engine::Engine::update(const GameTime& time, const input::InputState& inputState) {
	state.game->update(time, inputState);
}

void engine::Engine::draw(render::IRenderer& rendererBackend) {
    state.game->draw(rendererBackend);
}

bool engine::Engine::handleEngineEvent(EventCode code, void* sender, void* listenerInstance) {
    switch (code) {
        case EventCode::ApplicationQuit:
            LOG(LogLevel::Trace) << "EventCode::ApplicationQuit received, closing application.";
            state.isRunning = false;
            return true;
        default:
            break;
    }
    return false;
}