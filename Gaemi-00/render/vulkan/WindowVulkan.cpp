//
// Created by gaetz on 03/10/2022.
//

#include "WindowVulkan.h"
#include "../../Log.h"

using engine::render::vulkan::WindowVulkan;

WindowVulkan::WindowVulkan(str titleP) : title { titleP }
{
}

bool WindowVulkan::init(i32 x, i32 y, i32 width, i32 height, bool isFullscreen) {
    // Create SDL window pointer
    auto flags = static_cast<SDL_WindowFlags>(SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN);
    window = std::unique_ptr<SDL_Window, SDLWindowDestroyer>(
            SDL_CreateWindow(title.c_str(), x, y, width, height, flags)
    );

    // Check is window is initialized
    if (!window) {
        LOG(LogLevel::Fatal) << "Could not initialize window";
        return false;
    }

    LOG(LogLevel::Trace) << "Window initialized";
    return true;
}

void WindowVulkan::close() {
    SDL_Quit();
}

void WindowVulkan::updateFPSCounter(const GameTime& time) {
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