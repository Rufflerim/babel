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