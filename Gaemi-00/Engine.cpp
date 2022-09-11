#include "Engine.h"
#include "../Babel/Locator.h"
#include <SDL_events.h>
#include <SDL_image.h>

using engine::input::InputState;

engine::EngineState engine::Engine::state {};
engine::input::InputManager engine::Engine::inputManager { WINDOW_WIDTH, WINDOW_HEIGHT };
engine::Window engine::Engine::window{ "Babel" };
RendererSDL engine::Engine::renderer {};
engine::asset::AssetManager engine::Engine::assetManager {};
engine::Timer engine::Engine::timer {};
GameTime engine::Engine::time {};

void engine::Engine::init(IGame* game, ILocator& locator) {
	state.game = game;
    state.locator = &locator;

    // Init SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init( IMG_INIT_PNG | IMG_INIT_JPG );

	// Init everything
    bool inputsIgnited = inputManager.init(&locator);
	bool windowIgnited = window.init(WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, false);
    bool eventsIgnited = eventManager.init();
    eventManager.subscribe(EventCode::ApplicationQuit, nullptr, &onEngineEvent);
#ifndef GPLATFORM_WEB
    bool rendererIgnited = renderer.init(&locator, window);
    bool assetsIgnited = assetManager.init(renderer);
#else
    bool rendererIgnited { true };
    //bool rendererIgnited = renderer.init(&locator, window);
    bool assetsIgnited { true };
#endif

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
    loop();
    state.game->close();
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
    SDL_Quit();
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

void engine::Engine::update(const input::InputState& inputState) {
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

void engine::Engine::frame() {
    time = timer.computeTime(time);

    if (!state.game->isLoaded) {
        bool rendererIgnited = renderer.init(state.locator, window);
        bool assetsIgnited = assetManager.init(renderer);
        state.game->load();
    }

    // Inputs
    InputState inputState = inputs();

    // Update
    window.updateFPSCounter(time);
    update(inputState);

    // Draw
    renderer.clearScreen();
    renderer.beginDraw();
    draw(renderer);
    renderer.endDraw();

#ifndef GPLATFORM_WEB
    // Time delay if game loop is faster than target FPS
    timer.delayTime();
#endif
}

void engine::Engine::loop() {
#ifdef GPLATFORM_WEB
    //emscripten_set_main_loop_arg((em_arg_callback_func)frame, this, 0, 1);
    emscripten_set_main_loop(frame, 0, 1);
#else
    while (state.isRunning) {
        frame();
	}
#endif
}
