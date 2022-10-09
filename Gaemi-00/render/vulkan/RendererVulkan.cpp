//
// Created by gaetz on 03/10/2022.
//

#include "RendererVulkan.h"
#include "WindowVulkan.h"
#include "Instance.h"
#include "Device.h"
#include "Swapchain.h"
#include "Pipeline.h"
#include "../../Asserts.h"
#include "FrameBuffer.h"
#include "Commands.h"

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
    WindowVulkan& windowVulkan = dynamic_cast<WindowVulkan&>(window);
    instance = vkInit::makeInstance(debugMode, "Babel", windowVulkan);
    dynamicInstanceLoader = { instance, vkGetInstanceProcAddr };
    if (debugMode) debugMessenger = vkInit::makeDebugMessenger(instance, dynamicInstanceLoader);

    VkSurfaceKHR tmpSurface;
    SDL_Vulkan_CreateSurface(windowVulkan.get(), instance, &tmpSurface);
    surface = tmpSurface; // Cast through copy constructor

    physicalDevice = vkInit::choosePhysicalDevice(instance);
    GASSERT_MSG(nullptr != physicalDevice, "No suitable Vulkan physical device found. Exiting.");
    device = vkInit::createLogicalDevice(physicalDevice, surface, debugMode);
    GASSERT_MSG(nullptr != device, "Vulkan logical device could not be created. Exiting.");
    array<vk::Queue, 2> queues = vkInit::getQueues(physicalDevice, device, surface);
    graphicsQueue = queues.at(0);
    presentQueue = queues.at(1);

    vkInit::SwapchainBundle bundle = vkInit::createSwapchain(device, physicalDevice, surface,
                                                             windowVulkan.getBounds().size.x,
                                                             windowVulkan.getBounds().size.y);
    swapchain = bundle.swapchain;
    swapchainFrames = bundle.frames;
    swapchainFormat = bundle.format;
    swapchainExtent = bundle.extent;

    vkInit::GraphicsPipelineIn specifications {};
    specifications.device = device;
    specifications.vertexShaderPath = "Assets/shaders/first.vert.spv";
    specifications.fragmentShaderPath = "Assets/shaders/first.frag.spv";
    specifications.swapchainExtent = swapchainExtent;
    specifications.swapchainImageFormat = swapchainFormat;

    vkInit::GraphicsPipelineOut out = vkInit::makeGraphicsPipeline(specifications);
    layout = out.layout;
    renderPass = out.renderPass;
    pipeline = out.pipeline;

    vkInit::FramebufferInput framebufferInput;
    framebufferInput.device = device;
    framebufferInput.renderPass = renderPass;
    framebufferInput.extent = swapchainExtent;
    vkInit::makeFramebuffers(framebufferInput, swapchainFrames);

    commandPool = vkInit::makeCommandPool(device, physicalDevice, surface);
    vkInit::CommandBufferInput commandBufferInput { device, commandPool, swapchainFrames };
    mainCommandBuffer = vkInit::makeCommandBuffer(commandBufferInput);


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

    device.destroyCommandPool(commandPool);
    for (auto frame : swapchainFrames) {
        device.destroyFramebuffer(frame.framebuffer);
        device.destroyImageView(frame.imageView);
    }
    device.destroyPipeline(pipeline);
    device.destroyRenderPass(renderPass);
    device.destroyPipelineLayout(layout);
    device.destroySwapchainKHR(swapchain);
    device.destroy();
    instance.destroySurfaceKHR(surface);
    instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dynamicInstanceLoader);
    instance.destroy();
}

void
RendererVulkan::drawSprite(Texture* texture, const gmath::RectangleInt& srcRect, const gmath::RectangleInt& dstRect,
                           f64 angle, const gmath::Vec2& origin, engine::render::Flip flip) {
    LOG(LogLevel::Trace) << "Draw sprite request";
}

