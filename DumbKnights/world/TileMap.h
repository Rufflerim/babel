//
// Created by gaetz on 02/01/2022.
//

#ifndef DUMBKNIGHTS_TILEMAP_H
#define DUMBKNIGHTS_TILEMAP_H

#include <vector>
#include "TileInfo.h"
#include <Vec2i.h>

using std::vector;
using gmath::Vec2i;

namespace world {
    class TileMap {
    public:
        void loadMap(const str& path);
        void update(const GameTime& time);
        void draw(engine::render::IRenderer& rendererBackend);

        Tile& getTile(u32 x, u32 y) const;
        TileInfo& getDefaultTile() { return defaultTile; };
        const Vec2i& getMapSize() const { return mapSize; };
        const Vec2i& getPlayerStart() const { return playerStart; };

    private:
        vector<Tile> tileMap;
        vector<TileInfo> tileSet;
        Sprite background;
        TileInfo defaultTile;
        Vec2i mapSize;
        Vec2i playerStart;
        u32 tileCount;
        u32 tileSetCount;
    };
}


#endif //DUMBKNIGHTS_TILEMAP_H
