//
// Created by gaetz on 03/10/2022.
//

#ifndef RENDER_VULKAN_WINDOWVULKAN_H
#define RENDER_VULKAN_WINDOWVULKAN_H

#include "../IWindow.h"

#include <memory>
#include <SDL.h>
#include "../math/RectangleInt.h"

namespace engine::render::vulkan {

    struct SDLWindowDestroyer {
        void operator()(SDL_Window* window) const {
            SDL_DestroyWindow(window);
        }
    };

    class WindowVulkan : public IWindow
    {
    public:
        explicit WindowVulkan(str titleP);

        bool init(i32 x, i32 y, i32 width, i32 height, bool isFullscreen) override;

        const gmath::RectangleInt& getBounds() const override { return bounds; }

        SDL_Window* get() { return window.get(); }

        void close() override;

        void updateFPSCounter(const GameTime& time) override;

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


#endif //RENDER_VULKAN_WINDOWVULKAN_H
