//
// Created by gaetz on 03/10/2022.
//

#include "RendererVulkan.h"
#include "Instance.h"

using engine::render::vulkan::RendererVulkan;


bool RendererVulkan::init(engine::ILocator* locatorP, IWindow& window) {
    locator = locatorP;
    auto windowBounds = window.getBounds();
    width = windowBounds.size.x;
    height = windowBounds.size.y;

#ifdef DEBUG
    bool debugMode { true };
#else
    bool debugMode { false };
#endif
    instance = vkInit::makeInstance(debugMode, "Babel");

    LOG(LogLevel::Trace) << "Renderer:SDL initialized";
    return true;
}

void RendererVulkan::clearScreen() {

}

void RendererVulkan::beginDraw() {

}

void RendererVulkan::drawRectangle(const gmath::Rectangle& rectangle, const gmath::Color& color) {
    LOG(LogLevel::Trace) << "Draw rectangle request";
}

void RendererVulkan::endDraw() {

}

void RendererVulkan::close() {

}

void RendererVulkan::drawSprite(Texture* texture, const gmath::RectangleInt& srcRect, const gmath::RectangleInt& dstRect,
                           f64 angle, const gmath::Vec2& origin, engine::render::Flip flip) {
    LOG(LogLevel::Trace) << "Draw sprite request";
}
