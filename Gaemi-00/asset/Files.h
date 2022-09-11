//
// Created by gaetz on 11/09/2022.
//

#ifndef ASSET_FILES_H
#define ASSET_FILES_H

#include "../Defines.h"
#include <vector>

namespace engine::asset {

        enum class AssetType {
            Texture,
            Map,
            Tileset
        };

        class Files {
        public:
            /// Get the relative path to a file given its type and name
            /// \returns Relative file path
            /// \param assetType File type from enum AssetType
            /// \param fileName File name without extension
            static str getFilePath(AssetType assetType, const str& fileName);

            /// Get the lines from a file
            /// \returns Vector of text lines
            /// \param path Path to the file
            static std::vector<str> readFileLines(const str& path);
        };


    }
#endif //ASSET_FILES_H
