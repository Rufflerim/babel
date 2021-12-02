#pragma once
#include "pch.h"

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
		/// Update logic 
		/// </summary>
		/// <param name="time">Game time</param>
		void update(GameTime time);

		/// <summary>
		/// Draw everything
		/// </summary>
		void draw();

	private:
		EngineState state {};
	};

}