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
        explicit TileMap(const str& name);
        void load();
        void update(const GameTime& time);
        void draw(engine::render::IRenderer& rendererBackend);

        const Tile& getTile(u32 x, u32 y) const;
        const Vec2i& getMapSize() const { return mapSize; };
        const Vec2i& getPlayerStart() const { return playerStart; };

    private:
        void loadMap(const str& name);
        vector<Tile> tileMap;
        vector<TileInfo> tileSet;
        Sprite background;
        Vec2i mapSize;
        Vec2i playerStart;
        str name;
        i32 tileCount { 0 };
    };
}


#endif //DUMBKNIGHTS_TILEMAP_H
