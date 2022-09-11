//
// Created by gaetz on 11/09/2022.
//

#include "Files.h"
#include "../Log.h"

#ifdef GPLATFORM_WEB
#include <cstdio>
#else

#include <istream>

#endif

using engine::asset::Files;

using std::vector;
using std::ifstream;

str Files::getFilePath(AssetType assetType, const str& fileName) {
    switch (assetType) {
        case AssetType::Texture:
            return "Assets/textures/" + fileName + ".png";
        case AssetType::Map:
            return "Assets/data/" + fileName + ".map";
        case AssetType::Tileset:
            return "Assets/data/" + fileName + ".cfg";
    }
}

vector<str> Files::readFileLines(const str& path) {
    vector<str> lines;
#ifdef GPLATFORM_WEB
    // Emscripten only reads files the C way
    FILE* file { nullptr };
    file = fopen(path.c_str(), "r");
    if (!file) {
        LOG(LogLevel::Error) << "Could not load file " << path;
        return lines;
    }
    char* charLine { nullptr };
    size_t len { 0 };
    while (getline(&charLine, &len, file) != -1) {
        str line { charLine };
        lines.emplace_back( line );
    }
    fclose(file);
#else
    ifstream file { path };
    str line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
#endif
    return lines;
}