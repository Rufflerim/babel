//
// Created by gaetz on 02/01/2022.
//

#include "TileMap.h"
#include <asset/Files.h>

#include <utility>

using world::TileMap;
using world::Tile;
using engine::asset::Files;
using engine::asset::AssetType;
using std::vector;

TileMap::TileMap(str nameP) : name { std::move(nameP) } {
}

void TileMap::loadMap() {
    str tilesetName;

    // Load map
    str mapPath { Files::getFilePath(AssetType::Map, name) };
    vector<str> mapLines { Files::readFileLines(mapPath) };
    for (const auto& line: mapLines) {
        if (line[0] == '|') continue;
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
            for (i32 i = 0; i < tileCount; ++i) {
                Tile tileId;
                stream >> tileId;
                tileMap.emplace_back(tileId);
            }
        }
    }

    // Load tileset
    str tilesetPath { Files::getFilePath(AssetType::Tileset, tilesetName) };
    vector<str> tilesetLines { Files::readFileLines(tilesetPath) };
    for (const auto& line: tilesetLines) {
        if (line[0] == '|') continue;
        std::stringstream stream { line };
        i32 id;
        stream >> id;
        bool isBlocking;
        stream >> isBlocking;
        TileInfo info { id, isBlocking, tilesetName };
        tileSet.emplace_back(info);
    }
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
    for (auto tile: tileMap) {
        gmath::Vec2i worldCoords = gmath::convert1Dto2DCoords(index, mapSize.x) * TILE_SIZE;
        gmath::RectangleInt worldRect { worldCoords, { TILE_SIZE, TILE_SIZE }};
        rendererBackend.drawSprite(texture, tileSet[tile].sprite.srcRect, worldRect, 0, gmath::Vec2::zero());
        ++index;
    }
}

void world::TileMap::load() {
    loadMap();
}


