//
// Created by gaetz on 24/12/2021.
//

#include "GameMap.h"
#include <Log.h>

void scene::GameMap::onInit() {

}

void scene::GameMap::onClose() {

}

void scene::GameMap::activate() {

}

void scene::GameMap::inactivate() {

}

void scene::GameMap::update(GameTime time) {
    count++;
    if(count > 60) {
        LOG(engine::LogLevel::Debug) << "Count!";
        count = 0;
    }
}

void scene::GameMap::draw() {

}
