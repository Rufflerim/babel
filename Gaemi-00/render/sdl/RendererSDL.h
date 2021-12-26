//
// Created by gaetz on 26/12/2021.
//

#ifndef RENDER_SDL_RENDERERSDL_H
#define RENDER_SDL_RENDERERSDL_H

#include <SDL_render.h>
#include "../IRenderer.h"
#include "../../Window.h"
#include <memory>

namespace engine::render::sdl {
    struct SDLRendererDestroyer {
            void operator()(SDL_Renderer* renderer) const {
            SDL_DestroyRenderer(renderer);
        }
    };

    class RendererSDL : public IRenderer {
    public:
        RendererSDL() = default;
        bool init(Window& window) override;
        void clearScreen() override;
        void beginDraw() override;
        void drawRectangle(const Rectangle &rectangle, const Color &color) override;
        void endDraw() override;
        void close() override;

    private:
        std::unique_ptr<SDL_Renderer, SDLRendererDestroyer> renderer { nullptr };
        gmath::Color clearColor { gmath::Color::BLACK };
        i32 width { -1 };
        i32 height { -1 };
    };
}

#endif //RENDER_SDL_RENDERERSDL_H
