//
// Created by gaetz on 28/12/2021.
//

#ifndef RENDER_SDL_TEXTURE2D_H
#define RENDER_SDL_TEXTURE2D_H

#include "../../Defines.h"
#include <memory>
#include <SDL_render.h>

namespace engine { namespace render { namespace sdl {

    struct SDLTextureDestroyer {
        void operator()(SDL_Texture* texture) const {
            SDL_DestroyTexture(texture);
        }
    };

    struct Texture {
        Texture(str name, i32 width, i32 height, std::unique_ptr<SDL_Texture, SDLTextureDestroyer> sdlTexture);

        str name;
        i32 width;
        i32 height;
        std::unique_ptr<SDL_Texture, SDLTextureDestroyer> sdlTexture;
    };
} } }


#endif //RENDER_SDL_TEXTURE2D_H
