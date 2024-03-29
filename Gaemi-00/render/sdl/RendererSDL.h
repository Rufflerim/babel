//
// Created by gaetz on 26/12/2021.
//

#ifndef RENDER_SDL_RENDERERSDL_H
#define RENDER_SDL_RENDERERSDL_H

#include <SDL_render.h>
#include "../IRenderer.h"
#include "WindowSdl.h"
#include <memory>
#include "../Texture.h"

namespace engine::render::sdl {
    struct SDLRendererDestroyer {
        void operator()(SDL_Renderer* renderer) const {
            SDL_DestroyRenderer(renderer);
        }
    };

    class RendererSDL : public IRenderer {
    public:
        RendererSDL() = default;

        bool init(engine::ILocator* locator, IWindow& window) override;

        void load() override;

        void clearScreen() override;

        void beginDraw() override;

        void drawRectangle(const gmath::Rectangle& rectangle, const gmath::Color& color) override;

        void endDraw() override;

        void close() override;

        SDL_Renderer* getSdlRenderer() { return renderer.get(); }

        void drawSprite(Texture* texture, const gmath::RectangleInt& srcRect, const gmath::RectangleInt& dstRect,
                        f64 angle, const gmath::Vec2& origin, engine::render::Flip flip) override;

    private:
        ILocator* locator{nullptr};
        std::unique_ptr<SDL_Renderer, SDLRendererDestroyer> renderer{nullptr};
        gmath::Color clearColor{gmath::Color::BLACK};
        i32 width{-1};
        i32 height{-1};
    };
}

#endif //RENDER_SDL_RENDERERSDL_H
