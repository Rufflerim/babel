//
// Created by gaetz on 28/12/2021.
//

#include "AssetManager.h"
#include "../Log.h"
#include "../render/sdl/RendererSDL.h"
#include <sdl_stb_image.h>

using engine::render::sdl::SDLTextureDestroyer;

bool engine::asset::AssetManager::init(engine::render::IRenderer& renderer) {
    rendererRef = &renderer;
    LOG(LogLevel::Trace) << "Asset Manager initialized";
    return true;
}

void engine::asset::AssetManager::close() {
    textures.clear();
}


std::shared_ptr<engine::render::sdl::Texture> engine::asset::AssetManager::getTexture(const str& name) {
    return textures[name];
}

bool engine::asset::AssetManager::loadTexture(const str& path, const str& name) {
    bool result = true;
    // Load a pixel surface that fits the texture
    SDL_RWops *file = SDL_RWFromFile(path.c_str(), "rb");
    SDL_Surface *surf = STBIMG_Load_RW(file, 1);
    // In cas of error, creates a 256 * 256 magenta surface
    if (surf == nullptr) {
        LOG(LogLevel::Error) << "Could not load SDL texture: " << path << ". SDL Error:" << SDL_GetError();
        surf = SDL_CreateRGBSurface(0, 256, 256, 32, 0, 0, 0, 0);
        SDL_FillRect(surf, nullptr, SDL_MapRGB(surf->format, 255, 0, 255));
        result = false;
    }
    // Create a texture from the surface and store it as a shared pointer

    SDL_Texture* t = STBIMG_CreateTexture(
            reinterpret_cast<engine::render::sdl::RendererSDL*>(rendererRef)->getSdlRenderer(),
            static_cast<const unsigned char*>(surf->pixels), surf->w, surf->h, 4);

    if(t == nullptr) {
        LOG(LogLevel::Error) << SDL_GetError();
    }

    textures[name] = std::make_shared<render::sdl::Texture>(
            name, surf->w, surf->h,
            std::unique_ptr<SDL_PixelFormat>(surf->format),
            std::unique_ptr<SDL_Texture, SDLTextureDestroyer>(t)
    );
    // Close loading
    SDL_FreeSurface(surf);
    if (result) {
        LOG(LogLevel::Trace) << "Texture " << path << " loaded";
    }
    return result;
}
