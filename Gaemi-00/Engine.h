#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include "Defines.h"
#include "IGame.h"
#include "input/InputManager.h"
#include "Window.h"
#include "EventManager.h"
#include "render/sdl/RendererSDL.h"
#include "asset/AssetManager.h"
#include "ILocator.h"
#include "Timer.h"

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
        ErrorCode errorCode { ErrorCode::None };
		IGame* game { nullptr };
        ILocator* locator { nullptr };
	};

	class Engine
	{
	public:
		explicit Engine() = default;
		~Engine() = default;

		/// <summary>
		/// Init everything in the engine
		/// </summary>
		void init(IGame* game, ILocator& locator);

		/// <summary>
		/// Run the engine. Game loop.
		/// </summary>
		ErrorCode run();

		/// <summary>
		/// Shut down the engine
		/// </summary>
		void close();

		/// <summary>
		/// Process inputs with InputhManager
		/// </summary>
		/// <returns>Global inputs state</returns>
        static input::InputState inputs();

		/// <summary>
		/// Update logic 
		/// </summary>
		/// <param name="time">Game time</param>
        static void update(const GameTime& time, const input::InputState& inputState);

		/// <summary>
		/// Draw everything
		/// </summary>
        static void draw(render::IRenderer& rendererBackend);

	private:
        static Timer timer;
        static GameTime time;

        /// Game engine state
		static EngineState state;

        /// Management of inputs
		static input::InputManager inputManager;

        /// Global event management
        EventManager eventManager {};

        /// Game window
		static Window window;

        /// Engine renderer
        static RendererSDL renderer;

        /// Asset manager
        asset::AssetManager assetManager {};

        /// Callback for engine events
        EventCallback onEngineEvent = [this](EventCode code, void* sender, void* listInst) {
            return this->handleEngineEvent(code, sender, listInst);
        };

        /// Apply engine events management
        /// \param code Event type
        /// \param sender Event sender
        /// \param listenerInstance Event listener if specific
        /// \return True if an event occured
        bool handleEngineEvent(EventCode code, void* sender, void* listenerInstance);

        static void frame();
        void loop();
    };

}

#endif