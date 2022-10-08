//
// Created by gaetz on 03/10/2022.
//

#include "RendererVulkan.h"
#include "WindowVulkan.h"
#include "Instance.h"
#include "Device.h"
#include "../../Asserts.h"

using engine::render::vulkan::RendererVulkan;

bool RendererVulkan::init(engine::ILocator* locatorP, IWindow& window) {
    locator = locatorP;
    auto windowBounds = window.getBounds();
    width = windowBounds.size.x;
    height = windowBounds.size.y;

#ifdef GDEBUG
    bool debugMode { true };
#else
    bool debugMode { false };
#endif
    instance = vkInit::makeInstance(debugMode, "Babel", dynamic_cast<WindowVulkan&>(window));
    dynamicInstanceLoader = { instance, vkGetInstanceProcAddr };
    if (debugMode) debugMessenger = vkInit::makeDebugMessenger(instance, dynamicInstanceLoader);

    physicalDevice = vkInit::choosePhysicalDevice(instance);
    GASSERT_MSG(nullptr != physicalDevice, "No suitable physical device found. Exiting.");
    device = vkInit::createLogicalDevice(physicalDevice, debugMode);
    GASSERT_MSG(nullptr != device, "Vulkan logical device could not be created. Exiting.");
    graphicsQueue = vkInit::getQueue(physicalDevice, device);

    LOG(LogLevel::Trace) << "Renderer:Vulkan initialized";
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
    device.destroy();
    instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dynamicInstanceLoader);
    instance.destroy();
}

void RendererVulkan::drawSprite(Texture* texture, const gmath::RectangleInt& srcRect, const gmath::RectangleInt& dstRect,
                           f64 angle, const gmath::Vec2& origin, engine::render::Flip flip) {
    LOG(LogLevel::Trace) << "Draw sprite request";
}