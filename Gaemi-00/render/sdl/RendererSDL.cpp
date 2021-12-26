//
// Created by gaetz on 26/12/2021.
//

#include "RendererSDL.h"
#include "../../Log.h"

bool engine::render::sdl::RendererSDL::init(engine::Window &window) {
    width = static_cast<i32>(window.getBounds().size.x);
    height = static_cast<i32>(window.getBounds().size.y);
    renderer = std::unique_ptr<SDL_Renderer, SDLRendererDestroyer>(
            SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED)
    );

    LOG(LogLevel::Trace) << "Renderer:SDL initialized";
    return true;
}

void engine::render::sdl::RendererSDL::drawRectangle(const Rectangle &rectangle, const Color &color) {
    SDL_Rect fillRect = { static_cast<i32>(rectangle.offset.x),
                          static_cast<i32>(rectangle.offset.y),
                          static_cast<i32>(rectangle.size.x),
                          static_cast<i32>(rectangle.size.y)
    };
    SDL_SetRenderDrawColor( renderer.get(), color.r, color.g, color.b, color.a );
    SDL_RenderFillRect( renderer.get(), &fillRect );
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
