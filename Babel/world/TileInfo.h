//
// Created by gaetz on 02/01/2022.
//

#ifndef WORLD_TILEINFO_H
#define WORLD_TILEINFO_H

#include "../Defines.h"
#include "../components/Components.h"

constexpr i32 TILE_SIZE = 32;
constexpr i32 TILESHEET_WIDTH = 512;
constexpr i32 TILESHEET_HEIGHT = 512;

namespace world {
    /// Hold data and graphics information about a tileset element
    class TileInfo {
    public:
        TileInfo() = default;
        TileInfo(i32 idP, bool isBlockingP, const str& textureName);

        i32 id {0};
        bool isBlocking {false};
        Sprite sprite;
    };

    /// Tilemap elements are unsigned integers, corresponding to TileInfo::id
    using Tile = i32;
}


#endif //WORLD_TILEINFO_H
