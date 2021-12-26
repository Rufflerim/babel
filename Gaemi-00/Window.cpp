#include "Window.h"
#include "Defines.h"
#include "Log.h"
#include "GameTime.h"

engine::Window::Window(str titleP) : title { titleP }
{
}

bool engine::Window::init(i32 x, i32 y, i32 width, i32 height, bool isFullscreen)
{
	// Create SDL window pointer
	SDL_WindowFlags flags = SDL_WINDOW_SHOWN;
	window = std::unique_ptr<SDL_Window, SDLWindowDestroyer>(
		SDL_CreateWindow(title.c_str(), x, y, width, height, flags)
	);

	// Check is window is initialized
	if (!window) {
		LOG(LogLevel::Fatal) << "Could not initialize window";
		return false;
	}

	renderer = std::unique_ptr<SDL_Renderer, SDLRendererDestroyer>(
			SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED)
	);
	SDL_SetRenderDrawColor( renderer.get(), 0x00, 0x00, 0x00, 0xFF );

	LOG(LogLevel::Trace) << "Window initialized";
	return true;
}

void engine::Window::close()
{
}

void engine::Window::updateFPSCounter(const GameTime& time)
{
	f64 elapsed = time.totalTime - lastElapsed;
	if (elapsed > DURATION_BETWEEN_FPS_DISPLAY) {
		// Compute fps number
		f64 fps = static_cast<f64>(frameCount) / elapsed;
		
		// Compute text
		char text[128];
#ifdef GPLATFORM_WINDOWS
		sprintf_s(text, "%s @ fps: %.2f", title.c_str(), fps);
#else
		sprintf(text, "%s @ fps: %.2f", title.c_str(), fps);
#endif // GPLATFORM_WINDOWS

		// Display text and reset frame number
		SDL_SetWindowTitle(window.get(), text);
		frameCount = 0;
		lastElapsed = time.totalTime;
	}
	++frameCount;
}
