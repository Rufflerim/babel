//
// Created by gaetz on 28/12/2021.
//

#include "AssetManager.h"
#include "../render/sdl/RendererSDL.h"
#include <SDL_image.h>
#include "Files.h"

bool engine::asset::AssetManager::init(engine::render::IRenderer& renderer) {
    rendererRef = &renderer;
    LOG(LogLevel::Trace) << "Asset Manager initialized";
    return true;
}

void engine::asset::AssetManager::close() {
    textures.clear();
}


std::shared_ptr<engine::render::Texture> engine::asset::AssetManager::getTexture(const str& name) {
    return textures[name];
}

bool engine::asset::AssetManager::loadTexture(const str& name) {
    bool result = true;
    const str path { Files::getFilePath(AssetType::Texture, name) };

    // Load a pixel surface that fits the texture
    SDL_Surface* surf = IMG_Load(path.c_str());

    // In cas of error, creates a 256 * 256 magenta surface
    if (surf == nullptr) {
        LOG(LogLevel::Error) << "Could not load SDL surface: " << path << ". SDL Error: " << SDL_GetError();
        surf = SDL_CreateRGBSurface(0, 256, 256, 32, 0, 0, 0, 0);
        SDL_FillRect(surf, nullptr, SDL_MapRGB(surf->format, 255, 0, 255));
        result = false;
    }

    // Create a texture from the surface and store it as a shared pointer
#ifdef GPLATFORM_WEB
    SDL_Texture* t = SDL_CreateTextureFromSurface(
            reinterpret_cast<engine::render::sdl::RendererSDL*>(rendererRef)->getSdlRenderer(), surf);
    /*
    SDL_Texture* t = STBIMG_CreateTexture(
            reinterpret_cast<engine::render::sdl::RendererSDL*>(rendererRef)->getSdlRenderer(),
            static_cast<const unsigned char*>(surf->pixels), surf->w, surf->h, 4);
    */

    if (t == nullptr) {
        LOG(LogLevel::Error) << "Could not create SDL texture: " << path << ". SDL Error:" << SDL_GetError();
        result = false;
    }

    // Push texture into asset manager
    textures[name] = std::make_shared<render::Texture>(
            name, surf->w, surf->h,
            std::unique_ptr<SDL_Texture, engine::render::SDLTextureDestroyer>(t)
    );

    // Close loading
    SDL_FreeSurface(surf);
    if (result) {
        LOG(LogLevel::Trace) << "Texture " << path << " loaded";
    }
#else
    // Push texture into asset manager
    textures[name] = std::make_shared<render::Texture>(
            name, surf->w, surf->h,
            surf
    );
    LOG(LogLevel::Trace) << "Texture " << path << " loaded";
#endif
    return result;
}
