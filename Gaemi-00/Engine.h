#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include "Defines.h"
#include "IGame.h"
#include "input/InputManager.h"
#include "EventManager.h"
#include "asset/AssetManager.h"
#include "ILocator.h"
#include "Timer.h"

#ifdef GPLATFORM_WEB
#include <emscripten.h>
#include <emscripten/html5.h>

#include "render/sdl/RendererSDL.h"
#include "render/sdl/WindowSdl.h"
using engine::render::sdl::RendererSDL;
using engine::render::sdl::WindowSdl;
#else
#include "render/vulkan/RendererVulkan.h"
#include "render/vulkan/WindowVulkan.h"
using engine::render::vulkan::RendererVulkan;
using engine::render::vulkan::WindowVulkan;
#endif

constexpr i32 WINDOW_X = 100;
constexpr i32 WINDOW_Y = 100;

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

		/// Init everything in the engine
		void init(IGame* game, ILocator& locator);

#ifdef GPLATFORM_WEB
        /// Emscripten require to set up the renderer, and hence
        /// the asset manager, in the main loop. This function
        /// handle this initialization.
        static void emscriptenPostInit();
#endif

		/// Run the engine. Game loop.
		ErrorCode run();

        /// Contain a single frame logic
        static void frame();

        /// Launch game loop
        void loop();

		/// Shut down the engine
		void close();

		/// Process inputs with InputhManager
		/// \return Global inputs state
        static input::InputState inputs();

		/// Update logic
		/// \param inputState State of game's inputs
        static void update(const input::InputState& inputState);

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

#ifdef GPLATFORM_WEB
        /// Game window
		static WindowSdl window;

        /// Engine renderer
        static RendererSDL renderer;
#else
        /// Game window
        static WindowVulkan window;

        /// Engine renderer
        static RendererVulkan renderer;
#endif
        /// Asset manager
        static asset::AssetManager assetManager;

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
    };

}

#endif