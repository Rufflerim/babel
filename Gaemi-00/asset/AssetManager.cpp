//
// Created by gaetz on 28/12/2021.
//

#include "AssetManager.h"
#include "../Log.h"
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
    bool result = false;
    // Load a pixel surface that fits the texture
    SDL_Surface* surf = STBIMG_Load(path.c_str());
    // In cas of error, creates a 256 * 256 magenta surface
    if (surf == nullptr) {
        LOG(LogLevel::Error) << "Could not load SDL texture: " << path;
        surf = SDL_CreateRGBSurface(0, 256, 256, 32, 0, 0, 0, 0);
        SDL_FillRect(surf, nullptr, SDL_MapRGB(surf->format, 255, 0, 255));
    }
    // Create a texture from the surface and store it as a shared pointer
    textures[name] = std::make_shared<render::sdl::Texture>(
            name, surf->w, surf->h,
            std::unique_ptr<SDL_PixelFormat>(surf->format),
            std::unique_ptr<SDL_Texture, SDLTextureDestroyer>(STBIMG_CreateTexture(
                    reinterpret_cast<SDL_Renderer*>(rendererRef),
                    static_cast<const unsigned char*>(surf->pixels), surf->w, surf->h, 4)
            )
    );
    //
    SDL_FreeSurface(surf);
    result = true;
    return result;
}
