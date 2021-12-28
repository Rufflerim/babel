//
// Created by gaetz on 28/12/2021.
//

#ifndef ASSET_ASSETS_H
#define ASSET_ASSETS_H

#include "../Defines.h"
#include "../Gaemi.h"
#include "../render/sdl/Texture.h"

namespace engine::asset {
    class Assets {
    public:
        virtual ~Assets() = default;

        // Retrieves a stored SDL texture
        GAPI virtual std::shared_ptr<engine::render::sdl::Texture> getTexture(const str& name) = 0;

        // Loads (and generates) a SDL texture from file
        GAPI virtual bool loadTexture(const str& file, const str& name) = 0;
    };


    class NullAssets : public Assets {
    public:
        std::shared_ptr<engine::render::sdl::Texture> getTexture(const str& name) override;

        bool loadTexture(const str& file, const str& name) override;

    private:
        static void placeholderMessage() ;
    };
}

#endif //ASSET_ASSETS_H
