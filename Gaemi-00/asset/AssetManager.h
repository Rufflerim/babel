//
// Created by gaetz on 28/12/2021.
//

#ifndef ASSET_ASSETMANAGER_H
#define ASSET_ASSETMANAGER_H

#include <unordered_map>
#include "Assets.h"
#include "../render/IRenderer.h"
#include <memory>

using std::unordered_map;

namespace engine::asset {

    class AssetManager : public Assets {
    public:
        /// Init asset manager
        bool init(engine::render::IRenderer& rendererRef);

        /// Close asset manager and free memory
        void close();

        // Retrieves a stored texture
        std::shared_ptr<render::sdl::Texture> getTexture(const str& name) override;

        // Loads (and generates) a texture from file
        bool loadTexture(const str& file, const str& name) override;



    private:
        /// Reference to SDL renderer to load certain types of data
        engine::render::IRenderer* rendererRef { nullptr };

        /// Stores pointers to textures
        unordered_map<str, std::shared_ptr<render::sdl::Texture>> textures;

    };
}

#endif //ASSET_ASSETMANAGER_H
