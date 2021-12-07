#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include "Defines.h"
#include "GameTime.h"

#include <memory>
#include <SDL.h>

constexpr f64 DURATION_BETWEEN_FPS_DISPLAY = 0.25;

namespace engine {

	struct SDLWindowDestroyer {
		void operator()(SDL_Window* window) const {
			SDL_DestroyWindow(window);
		}
	};

	class Window
	{
	public:
		explicit Window(str titleP);

		/// <summary>
		/// Initilize window with correct size and position
		/// </summary>
		/// <param name="x">Window horizontal position</param>
		/// <param name="y">Windows vertical position</param>
		/// <param name="width">Window's width</param>
		/// <param name="height">Window's hight</param>
		/// <param name="isFullscreen">True is window is fullscreen</param>
		/// <returns>True when properly initialized</returns>
		bool init(i32 x, i32 y, i32 width, i32 height, bool isFullscreen);

		/// <summary>
		/// Clean up window
		/// </summary>
		void close();

		/// <summary>
		/// Write FPS number on window
		/// </summary>
		/// <param name="time">Game time</param>
		void updateFPSCounter(const GameTime& time);

		/// <summary>
		/// Return SDL_Window raw pointer when needed
		/// </summary>
		/// <returns>Raw SDL_Window pointer</returns>
		SDL_Window* get() { return window.get(); }




	private:
		/// <summary>
		/// Pointer to SDL window
		/// </summary>
		std::unique_ptr<SDL_Window, SDLWindowDestroyer> window;

		/// <summary>
		/// Window title
		/// </summary>
		str title { "" };

		/// <summary>
		/// Last total number of seconds when FPS was display
		/// </summary>
		f64 lastElapsed { 0.0 };

		/// <summary>
		/// Number of frame since last FPS display
		/// </summary>
		i32 frameCount { 0 };

	};

}

#endif