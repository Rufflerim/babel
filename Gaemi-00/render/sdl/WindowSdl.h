#pragma once
#ifndef RENDER_SDL_WINDOWSDL_H
#define RENDER_SDL_WINDOWSDL_H

#include "../IWindow.h"

#include <memory>
#include <SDL.h>
#include "../math/RectangleInt.h"

namespace engine::render::sdl {

	struct SDLWindowDestroyer {
		void operator()(SDL_Window* window) const {
			SDL_DestroyWindow(window);
		}
	};
/*
    struct SDLRendererDestroyer {
        void operator()(SDL_Renderer* renderer) const {
            SDL_DestroyRenderer(renderer);
        }
    };
*/
	class WindowSdl : public IWindow
	{
	public:
		explicit WindowSdl(str titleP);

		bool init(i32 x, i32 y, i32 width, i32 height, bool isFullscreen) override;

		void close() override;

		void updateFPSCounter(const GameTime& time) override;

        const gmath::RectangleInt& getBounds() const override { return bounds; }

        /// <summary>
		/// Return SDL_Window raw pointer when needed
		/// </summary>
		/// <returns>Raw SDL_Window pointer</returns>
		SDL_Window* get() { return window.get(); }

	private:
		/// Pointer to SDL window
		std::unique_ptr<SDL_Window, SDLWindowDestroyer> window;

		/// Window title
		str title;

		/// Last total number of seconds when FPS was display
		f64 lastElapsed { 0.0 };

		/// Number of frame since last FPS display
		i32 frameCount { 0 };

        /// Window bounds
        gmath::RectangleInt bounds;
	};

}

#endif // RENDER_SDL_WINDOWSDL_H
