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
#include "Sync.h"

using engine::render::vulkan::RendererVulkan;
using gmath::Color;

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
    inFlightFence = vkInit::makeFence(device);
    imageAvailable = vkInit::makeSemaphore(device);
    renderFinished = vkInit::makeSemaphore(device);

    LOG(LogLevel::Trace) << "Renderer:Vulkan initialized";
    return true;
}

void RendererVulkan::clearScreen() {

}

void RendererVulkan::beginDraw() {
    render();
}

void RendererVulkan::drawRectangle(const gmath::Rectangle& rectangle, const gmath::Color& color) {
    //LOG(LogLevel::Trace) << "Draw rectangle request";
}

void RendererVulkan::endDraw() {

}

void RendererVulkan::close() {
    device.waitIdle();

    device.destroySemaphore(renderFinished);
    device.destroySemaphore(imageAvailable);
    device.destroyFence(inFlightFence);
    device.destroyCommandPool(commandPool);
    for (auto frame: swapchainFrames) {
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
    //LOG(LogLevel::Trace) << "Draw sprite request";
}

void RendererVulkan::recordDrawCommands(vk::CommandBuffer commandBuffer, u32 imageIndex) {
    vk::CommandBufferBeginInfo beginInfo {};
    commandBuffer.begin(beginInfo);

    vk::RenderPassBeginInfo renderPassBeginInfo {};
    renderPassBeginInfo.renderPass = renderPass;
    renderPassBeginInfo.framebuffer = swapchainFrames[imageIndex].framebuffer;
    renderPassBeginInfo.renderArea.offset.x = 0;
    renderPassBeginInfo.renderArea.offset.y = 0;
    renderPassBeginInfo.renderArea.extent = swapchainExtent;
    vk::ClearValue vkClearColor {clearColor.toFloatArray() };
    renderPassBeginInfo.clearValueCount = 1;
    renderPassBeginInfo.pClearValues = &vkClearColor;

    commandBuffer.beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline);
    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
    commandBuffer.draw(3, 1, 0, 0);
    commandBuffer.endRenderPass();
    commandBuffer.end();
}

void RendererVulkan::render() {
    // Wait for the images to sent in queue
    auto waitForInFlightResult = device.waitForFences(1, &inFlightFence, VK_TRUE, UINT64_MAX);
    auto resetInFlightResult = device.resetFences(1, &inFlightFence);

    // When image is acquired, semaphore availableImage is signaled
    u32 imageIndex { device.acquireNextImageKHR(swapchain, UINT64_MAX, imageAvailable, nullptr).value };

    vk::CommandBuffer commandBuffer = swapchainFrames[imageIndex].commandBuffer;
    commandBuffer.reset();
    recordDrawCommands(commandBuffer, imageIndex);

    vk::SubmitInfo submitInfo {};
    // We wait for the image to be available...
    vk::Semaphore waitSemaphores[] { imageAvailable };
    // ...and we wait before the color is output
    vk::PipelineStageFlags waitStages[] { vk::PipelineStageFlagBits::eColorAttachmentOutput };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    // This semaphore will be signaled after the command buffer is complete
    vk::Semaphore signalSemaphores[] { renderFinished };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    // Submit to the graphics queue to execute rendering
    graphicsQueue.submit(submitInfo, inFlightFence);

    // Send to present queue when rendering is finished
    vk::PresentInfoKHR presentInfo {};
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    vk::SwapchainKHR swapchains[] { swapchain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapchains;
    presentInfo.pImageIndices = &imageIndex;
    auto presentResult = presentQueue.presentKHR(presentInfo);
}