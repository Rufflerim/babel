#pragma once
#include "Timer.h"

namespace engine {

	class IGame {
	public:
		/// <summary>
		/// Game load
		/// </summary>
		virtual void load() = 0;

		/// <summary>
		/// Update state of the world
		/// </summary>
		/// <param name="time">Game time</param>
		virtual void update(GameTime time) = 0;

		/// <summary>
		/// Renders the world
		/// </summary>
		virtual void draw() = 0;

		/// <summary>
		/// End game
		/// </summary>
		virtual void close() = 0;
	};

}