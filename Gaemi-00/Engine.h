#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include "Defines.h"
#include "IGame.h"
#include "input/InputManager.h"
#include "Window.h"
#include "EventManager.h"
#include "render/sdl/RendererSDL.h"

using engine::render::sdl::RendererSDL;

constexpr i32 WINDOW_X = SDL_WINDOWPOS_CENTERED;
constexpr i32 WINDOW_Y = SDL_WINDOWPOS_CENTERED;
constexpr i32 WINDOW_WIDTH = 1280;
constexpr i32 WINDOW_HEIGHT = 720;

namespace engine {

	struct EngineState {
		bool isInitialized { false };
		bool isRunning { false };
		bool isPaused { false };
		IGame* game;
	};

	class Engine
	{
	public:
		explicit Engine() = default;
		~Engine() = default;

		/// <summary>
		/// Init everything in the engine
		/// </summary>
		void init(IGame& game);

		/// <summary>
		/// Run the engine. Game loop.
		/// </summary>
		void run();

		/// <summary>
		/// Shut down the engine
		/// </summary>
		void close();

		/// <summary>
		/// Process inputs with InputhManager
		/// </summary>
		/// <returns>Global inputs state</returns>
		input::InputState inputs();

		/// <summary>
		/// Update logic 
		/// </summary>
		/// <param name="time">Game time</param>
		void update(GameTime time);

		/// <summary>
		/// Draw everything
		/// </summary>
        void draw(render::IRenderer& rendererBackend);

	private:
        /// Game engine state
		EngineState state {};

        /// Management of inputs
		input::InputManager inputManager { WINDOW_WIDTH, WINDOW_HEIGHT };

        /// Global event management
        EventManager eventManager {};

        /// Game window
		Window window{ "Dumb Knights " };

        /// Engine renderer
        RendererSDL renderer {};

        /// Callback for engine events
        EventCallback onEngineEvent = [this](EventCode code, void* sender, void* listInst, EventContext context) {
            return this->handleEngineEvent(code, sender, listInst, context);
        };

        /// Apply engine events management
        /// \param code Event type
        /// \param sender Event sender
        /// \param listenerInstance Event listener if specific
        /// \param context Event additional data
        /// \return True if an event occured
        bool handleEngineEvent(EventCode code, void* sender, void* listenerInstance, EventContext context);
	};

}

#endif