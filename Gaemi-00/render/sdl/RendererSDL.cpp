//
// Created by gaetz on 26/12/2021.
//

#include "RendererSDL.h"
#include "../../Log.h"
#include "../../ILocator.h"
#include "../math/Vec2.h"
#include "../math/Rectangle.h"
#include "../math/RectangleInt.h"

using engine::render::sdl::RendererSDL;
using gmath::Rectangle;
using gmath::RectangleInt;
using gmath::Vec2;
using gmath::Color;

bool engine::render::sdl::RendererSDL::init(engine::ILocator* locatorP, engine::Window& window) {
    locator = locatorP;
    auto windowBounds = window.getBounds();
    width = windowBounds.size.x;
    height = windowBounds.size.y;
    renderer = std::unique_ptr<SDL_Renderer, SDLRendererDestroyer>(
            SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED)
    );

    LOG(LogLevel::Trace) << "Renderer:SDL initialized";
    return true;
}

void engine::render::sdl::RendererSDL::drawRectangle(const Rectangle& rectangle, const Color& color) {
    SDL_Rect fillRect = rectangle.toSdlRect();
    SDL_SetRenderDrawColor(renderer.get(), color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer.get(), &fillRect);
}

void engine::render::sdl::RendererSDL::clearScreen() {
    SDL_SetRenderDrawColor(renderer.get(), clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    SDL_RenderClear(renderer.get());
}

void engine::render::sdl::RendererSDL::beginDraw() {

}

void engine::render::sdl::RendererSDL::endDraw() {
    SDL_RenderPresent(renderer.get());
}

void engine::render::sdl::RendererSDL::close() {

}

void RendererSDL::drawSprite(Texture* texture, const RectangleInt& srcRect, const RectangleInt& dstRect,
                             f64 angle, const Vec2& origin, engine::render::Flip flip) {
    SDL_RendererFlip sdlFlip = SDL_FLIP_NONE;
    switch (flip) {
        case Flip::Horizontal:
            sdlFlip = SDL_FLIP_HORIZONTAL;
            break;
        case Flip::Vertical:
            sdlFlip = SDL_FLIP_VERTICAL;
            break;
        case Flip::HorizontalAndVertical:
            sdlFlip = static_cast<SDL_RendererFlip>(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
            break;
        default:
            sdlFlip = SDL_FLIP_NONE;
            break;
    }
    const SDL_Point center {static_cast<i32>(origin.x), static_cast<i32>(origin.y)};

    SDL_Rect srcSdlRect = srcRect.toSdlRect();
    SDL_Rect* srcFinalRect = srcRect == gmath::RectangleInt::nullRectangle ? nullptr : &srcSdlRect;

    SDL_Rect dstSdlRect = dstRect.toSdlRect();
    SDL_Rect* dstFinalRect = dstRect == gmath::RectangleInt::nullRectangle ? nullptr : &dstSdlRect;

    SDL_RenderCopyEx(renderer.get(), texture->sdlTexture.get(),
                     srcFinalRect, dstFinalRect, angle, &center, sdlFlip);
}
