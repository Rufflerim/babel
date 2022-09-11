//
// Created by gaetz on 02/01/2022.
//

#include "TileMap.h"
#include <stdio.h>

using world::TileMap;
using world::Tile;

TileMap::TileMap(const str& nameP) : name(nameP) {
}

void TileMap::loadMap(const str& name) {
    // Load map
    str mapPath { "Assets/data/" + name + ".map" };
    str tilesetName;
    FILE* mapFile = fopen(mapPath.c_str(), "r");
    if (!mapFile) {
        LOG(LogLevel::Error) << "Could not load map " << mapPath;
        return;
    }
    char* charLine { nullptr };
    size_t len { 0 };
    while (getline(&charLine, &len, mapFile) != -1) {
        str line { charLine };
        if(line[0] == '|') continue;
        std::stringstream stream { line };
        str type;
        stream >> type;
        if (type == "TILESET") {
            stream >> tilesetName;
        } else if (type == "BACKGROUND") {
            str backgroundName;
            stream >> backgroundName;
            background = Sprite { backgroundName, gmath::Vec2::zero(), gmath::RectangleInt::nullRectangle };
        } else if (type == "SIZE") {
            i32 sizeX, sizeY;
            stream >> sizeX;
            stream >> sizeY;
            tileCount = sizeX * sizeY;
            tileMap.reserve(tileCount);
            mapSize = { sizeX, sizeY };
        } else if (type == "PLAYER_START") {
            i32 startX, startY;
            stream >> startX;
            stream >> startY;
            playerStart = { startX, startY };
        } else if (type == "TILES") {
            for(i32 i = 0; i < tileCount; ++i) {
                Tile tileId;
                stream >> tileId;
                tileMap.push_back(tileId);
            }
        }
    }
    fclose(mapFile);

    // Load tileset
    str tilesetPath { "Assets/data/" + tilesetName + ".cfg"};
    FILE* tilesetFile = fopen(tilesetPath.c_str(), "r");
    if (!tilesetFile) {
        LOG(LogLevel::Error) << "Could not load tileset " << tilesetPath;
        return;
    }
    while (getline(&charLine, &len, tilesetFile) != -1) {
        str line { charLine };
        if(line[0] == '|') continue;
        std::stringstream stream { line };
        i32 id;
        stream >> id;
        bool isBlocking;
        stream >> isBlocking;
        TileInfo info { id, isBlocking, tilesetName };
        tileSet.push_back(info);
    }
    fclose(tilesetFile);
}

void TileMap::update(const GameTime& time) {

}

const Tile& TileMap::getTile(u32 x, u32 y) const {
    return tileMap[gmath::convert2Dto1DCoords(x, y, mapSize.x)];
}

void world::TileMap::draw(engine::render::IRenderer& rendererBackend) {
    i32 index { 0 };
    // Draw background
    auto bgTexture = Locator::instance().assets().getTexture(background.textureName).get();
    gmath::RectangleInt dstRect { gmath::Vec2i::zero(), mapSize * TILE_SIZE };
    rendererBackend.drawSprite(bgTexture, gmath::RectangleInt::nullRectangle, dstRect, 0, gmath::Vec2::zero());

    // All tiles have same texture
    auto texture = Locator::instance().assets().getTexture(tileSet[0].sprite.textureName).get();
    // Draw tiles in order
    for(auto tile : tileMap) {
        gmath::Vec2i worldCoords = gmath::convert1Dto2DCoords(index, mapSize.x) * TILE_SIZE;
        gmath::RectangleInt worldRect { worldCoords, { TILE_SIZE, TILE_SIZE } };
        rendererBackend.drawSprite(texture, tileSet[tile].sprite.srcRect, worldRect, 0, gmath::Vec2::zero());
        ++index;
    }
}

void world::TileMap::load() {
    loadMap(name);
}


