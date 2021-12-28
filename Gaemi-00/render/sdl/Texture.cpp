//
// Created by gaetz on 28/12/2021.
//

#include "Texture.h"

#include <utility>

engine::render::sdl::Texture::Texture(str name, i32 width, i32 height,
                                      std::unique_ptr<SDL_PixelFormat> pixelFormat,
                                      std::unique_ptr<SDL_Texture, SDLTextureDestroyer> sdlTexture) :
        name{std::move(name)}, width{width}, height{height}, pixelFormat{std::move(pixelFormat)},
        sdlTexture{std::move(sdlTexture)} {

}
