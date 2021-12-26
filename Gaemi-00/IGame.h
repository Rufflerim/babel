#pragma once
#ifndef IGAME_H
#define IGAME_H

#include <SDL_render.h>
#include "GameTime.h"

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
		virtual void draw(SDL_Renderer *pRenderer) = 0;

        /// <summary>
        /// Updates what need to be updated after drawing
        /// </summary>
        virtual void lateUpdate() = 0;

		/// <summary>
		/// End game
		/// </summary>
		virtual void close() = 0;
	};

}

#endif