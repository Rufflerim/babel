//
// Created by gaetz on 02/01/2022.
//

#ifndef WORLD_TILEINFO_H
#define WORLD_TILEINFO_H

#include "../Defines.h"
#include "../components/Components.h"
#include <Rectangle.h>

constexpr u32 TILE_SIZE = 32;
constexpr u32 TILESHEET_WIDTH = 512;
constexpr u32 TILESHEET_HEIGHT = 512;

namespace world {
    /// Hold data and graphics information about a tileset element
    class TileInfo {
    public:
        TileInfo() = default;
        TileInfo(u32 idP, bool isBlockingP, const str& textureName);

        u32 id {0};
        bool isBlocking {false};
        Sprite sprite;
    };

    /// Hold data about a tilemap element
    struct Tile {
        explicit Tile(const TileInfo& tileInfoP) : tileInfo { tileInfoP} {}

        const TileInfo& tileInfo;
    };
}


#endif //WORLD_TILEINFO_H
