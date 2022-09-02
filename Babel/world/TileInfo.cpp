//
// Created by gaetz on 02/01/2022.
//

#include "TileInfo.h"
#include <RectangleInt.h>

using world::TileInfo;
using gmath::RectangleInt;

TileInfo::TileInfo(i32 idP, bool isBlockingP, const str& textureName)
        : id { idP }, isBlocking { isBlockingP },
          sprite { textureName, Vec2::zero(), RectangleInt::nullRectangle, Vec2 { TILESHEET_WIDTH, TILESHEET_HEIGHT },
                   engine::render::Flip::None } {
    // Set src rect in function of the tile id
    sprite.srcRect = {{ id % (TILESHEET_WIDTH / TILE_SIZE) * TILE_SIZE,
                                   id / (TILESHEET_HEIGHT / TILE_SIZE) * TILE_SIZE },
                      { TILE_SIZE, TILE_SIZE }};
}
