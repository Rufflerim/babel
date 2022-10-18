//
// Created by gaetz on 28/12/2021.
//

#include "Texture.h"
#include <utility>

#ifdef GPLATFORM_WEB
engine::render::Texture::Texture(str name, i32 width, i32 height,
                                      std::unique_ptr<SDL_Texture, SDLTextureDestroyer> sdlTexture) :
        name{std::move(name)}, width{width}, height{height},
        sdlTexture{std::move(sdlTexture)} {

}
#else
engine::render::Texture::Texture(str name, i32 width, i32 height, SDL_Surface* surface) :
        name{std::move(name)}, width{width}, height{height},
        bitmap {surface} {

}

engine::render::Texture::~Texture() {
    SDL_FreeSurface(bitmap);
}
#endif

