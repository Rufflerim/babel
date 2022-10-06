//
// Created by gaetz on 28/12/2021.
//

#include "Assets.h"
#include "../Log.h"

using engine::asset::NullAssets;
using engine::render::Texture;

std::shared_ptr<Texture> NullAssets::getTexture(const str& name) {
    placeholderMessage();
    return nullptr;
}

bool engine::asset::NullAssets::loadTexture(const str& name) {
    placeholderMessage();
    return true;
}

void engine::asset::NullAssets::placeholderMessage() {
    LOG(LogLevel::Warning) << "Usage of placeholder assets service.";
}
