//
// Created by gaetz on 02/01/2022.
//

#include "TileInfo.h"

using world::TileInfo;

TileInfo::TileInfo(u32 idP, bool isBlockingP, const str& textureName)
        : id { idP }, isBlocking { isBlockingP },
          sprite { textureName, Vec2::zero(), Rectangle::nullRectangle, Vec2 { TILESHEET_WIDTH, TILESHEET_HEIGHT },
                   engine::render::Flip::None } {
    // Set src rect in function of the tile id
    sprite.srcRect = {{ static_cast<f32>(id % (TILESHEET_WIDTH / TILE_SIZE) * TILE_SIZE),
                                   static_cast<f32>(id / (TILESHEET_HEIGHT / TILE_SIZE) * TILE_SIZE) },
                      { TILE_SIZE, TILE_SIZE }};
}
