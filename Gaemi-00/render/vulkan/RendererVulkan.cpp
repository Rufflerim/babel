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

    makeSwapchain();
    currentFrameNumber = 0;

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

    makeFramebuffers();
    commandPool = vkInit::makeCommandPool(device, physicalDevice, surface);
    vkInit::CommandBufferInput commandBufferInput { device, commandPool, swapchainFrames };
    mainCommandBuffer = vkInit::makeCommandBuffer(commandBufferInput);
    vkInit::makeFrameCommandBuffers(commandBufferInput);
    makeSyncObjects();

    makeAssets();

    LOG(LogLevel::Trace) << "Renderer:Vulkan initialized";
    return true;
}

void RendererVulkan::clearScreen() {

}

void RendererVulkan::beginDraw() {
    render(testScene);
}

void RendererVulkan::drawRectangle(const gmath::Rectangle& rectangle, const gmath::Color& color) {
    //LOG(LogLevel::Trace) << "Draw rectangle request";
}

void RendererVulkan::endDraw() {

}

void RendererVulkan::close() {
    auto waitRes = device.waitIdle();

    device.destroyCommandPool(commandPool);
    closeSwapchain();
    device.destroyPipeline(pipeline);
    device.destroyRenderPass(renderPass);
    device.destroyPipelineLayout(layout);

    delete geometryMeshes;

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

void RendererVulkan::recordDrawCommands(vk::CommandBuffer commandBuffer, u32 imageIndex, TestScene& testScene) {
    vk::CommandBufferBeginInfo beginInfo {};
    auto commandBufferBeginRes = commandBuffer.begin(beginInfo);

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

    prepareScene(commandBuffer);
    i32 firstVertex = geometryMeshes->getFirstVertex(vkMesh::GeometryType::Triangle);
    i32 vertexCount = geometryMeshes->getVertexCount(vkMesh::GeometryType::Triangle);
    for (glm::vec3 position : testScene.trianglePositions) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
        vkUtils::ObjectData objectData {};
        objectData.model = model;
        commandBuffer.pushConstants(
                layout, vk::ShaderStageFlagBits::eVertex,
                0, sizeof(objectData), &objectData
        );
        commandBuffer.draw(vertexCount, 1, firstVertex, 0);
    }

    firstVertex = geometryMeshes->getFirstVertex(vkMesh::GeometryType::Square);
    vertexCount = geometryMeshes->getVertexCount(vkMesh::GeometryType::Square);
    for (glm::vec3 position : testScene.squarePositions) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
        vkUtils::ObjectData objectData {};
        objectData.model = model;
        commandBuffer.pushConstants(
                layout, vk::ShaderStageFlagBits::eVertex,
                0, sizeof(objectData), &objectData
        );
        commandBuffer.draw(vertexCount, 1, firstVertex, 0);
    }

    firstVertex = geometryMeshes->getFirstVertex(vkMesh::GeometryType::Star);
    vertexCount = geometryMeshes->getVertexCount(vkMesh::GeometryType::Star);
    for (glm::vec3 position : testScene.starPositions) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
        vkUtils::ObjectData objectData {};
        objectData.model = model;
        commandBuffer.pushConstants(
                layout, vk::ShaderStageFlagBits::eVertex,
                0, sizeof(objectData), &objectData
        );
        commandBuffer.draw(vertexCount, 1, firstVertex, 0);
    }


    for(auto& position : testScene.trianglePositions) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
        vkUtils::ObjectData objectData;
        objectData.model = model;
        commandBuffer.pushConstants(layout, vk::ShaderStageFlagBits::eVertex, 0, sizeof(vkUtils::ObjectData), &objectData);
        commandBuffer.draw(3, 1, 0, 0);

    }

    commandBuffer.endRenderPass();
    auto commandBufferBeginEnd = commandBuffer.end();
}

void RendererVulkan::render(TestScene& testScene) {
    // Wait for the images to sent in queue
    auto waitForInFlightResult = device.waitForFences(1, &swapchainFrames[currentFrameNumber].inFlightFence, VK_TRUE, UINT64_MAX);

    // When image is acquired, semaphore availableImage is signaled
    vk::ResultValue acquireRes = device.acquireNextImageKHR(swapchain, UINT64_MAX, swapchainFrames[currentFrameNumber].imageAvailable, nullptr);
    if (acquireRes.result == vk::Result::eErrorOutOfDateKHR) {
        recreateSwapchain();
        return;
    }
    u32 imageIndex = acquireRes.value;

    vk::CommandBuffer commandBuffer = swapchainFrames[currentFrameNumber].commandBuffer;
    commandBuffer.reset();
    recordDrawCommands(commandBuffer, imageIndex, testScene);

    vk::SubmitInfo submitInfo {};
    // We wait for the image to be available...
    vk::Semaphore waitSemaphores[] { swapchainFrames[currentFrameNumber].imageAvailable };
    // ...and we wait before the color is output
    vk::PipelineStageFlags waitStages[] { vk::PipelineStageFlagBits::eColorAttachmentOutput };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    // This semaphore will be signaled after the command buffer is complete
    vk::Semaphore signalSemaphores[] { swapchainFrames[currentFrameNumber].renderFinished };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    auto resetInFlightRes = device.resetFences(1, &swapchainFrames[currentFrameNumber].inFlightFence);

    // Submit to the graphics queue to execute rendering
    auto graphicsQueueSubmitRes = graphicsQueue.submit(submitInfo, swapchainFrames[currentFrameNumber].inFlightFence);

    // Send to present queue when rendering is finished
    vk::PresentInfoKHR presentInfo {};
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    vk::SwapchainKHR swapchains[] { swapchain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapchains;
    presentInfo.pImageIndices = &imageIndex;

    auto presentRes = presentQueue.presentKHR(presentInfo);
    if (presentRes == vk::Result::eErrorOutOfDateKHR || presentRes == vk::Result::eSuboptimalKHR) {
        recreateSwapchain();
        return;
    }

    // Switch to next frame
    currentFrameNumber = (currentFrameNumber + 1) % maxFrameInFlight;
}

void engine::render::vulkan::RendererVulkan::makeSwapchain() {
    vkInit::SwapchainBundle bundle = vkInit::createSwapchain(device, physicalDevice, surface,
                                                             width, height);
    swapchain = bundle.swapchain;
    swapchainFrames = bundle.frames;
    swapchainFormat = bundle.format;
    swapchainExtent = bundle.extent;
    maxFrameInFlight = static_cast<i32>(swapchainFrames.size());
}

void engine::render::vulkan::RendererVulkan::closeSwapchain() {
    for (auto frame: swapchainFrames) {
        device.destroyFramebuffer(frame.framebuffer);
        device.destroyImageView(frame.imageView);
        device.destroySemaphore(frame.renderFinished);
        device.destroySemaphore(frame.imageAvailable);
        device.destroyFence(frame.inFlightFence);
    }
    device.destroySwapchainKHR(swapchain);
}

void engine::render::vulkan::RendererVulkan::recreateSwapchain() {
    auto waitRes = device.waitIdle();
    closeSwapchain();
    makeSwapchain();
    makeFramebuffers();
    makeSyncObjects();
    vkInit::CommandBufferInput commandBufferInput { device, commandPool, swapchainFrames };
    vkInit::makeFrameCommandBuffers(commandBufferInput);
}

void engine::render::vulkan::RendererVulkan::makeFramebuffers() {
    vkInit::FramebufferInput framebufferInput;
    framebufferInput.device = device;
    framebufferInput.renderPass = renderPass;
    framebufferInput.extent = swapchainExtent;
    vkInit::makeFramebuffers(framebufferInput, swapchainFrames);
}

void engine::render::vulkan::RendererVulkan::makeSyncObjects() {
    for (auto& frame : swapchainFrames) {
        frame.inFlightFence = vkInit::makeFence(device);
        frame.imageAvailable = vkInit::makeSemaphore(device);
        frame.renderFinished = vkInit::makeSemaphore(device);
    }
}

void engine::render::vulkan::RendererVulkan::makeAssets() {
    geometryMeshes = new vkMesh::VertexBufferAtlas();
    vector<float> vertices {{
        // Position         // Color
        0.0f, -0.05f, 0.0f, 1.0f, 0.0f,
        0.05f, 0.05f, 0.0f, 1.0f, 0.0f,
        -0.05f, 0.05f, 0.0f, 1.0f, 0.0f
    }};
    geometryMeshes->consume(vkMesh::GeometryType::Triangle, vertices);

    vertices.clear();
    vertices = { {
        -0.05f,  0.05f, 1.0f, 0.0f, 0.0f,
        -0.05f, -0.05f, 1.0f, 0.0f, 0.0f,
        0.05f, -0.05f, 1.0f, 0.0f, 0.0f,
        0.05f, -0.05f, 1.0f, 0.0f, 0.0f,
        0.05f,  0.05f, 1.0f, 0.0f, 0.0f,
        -0.05f,  0.05f, 1.0f, 0.0f, 0.0f
    } };
    geometryMeshes->consume(vkMesh::GeometryType::Square, vertices);

    vertices.clear();
    vertices = { {
         -0.05f, -0.025f, 0.0f, 0.0f, 1.0f,
         -0.02f, -0.025f, 0.0f, 0.0f, 1.0f,
         -0.03f,    0.0f, 0.0f, 0.0f, 1.0f,
         -0.02f, -0.025f, 0.0f, 0.0f, 1.0f,
         0.0f,  -0.05f, 0.0f, 0.0f, 1.0f,
         0.02f, -0.025f, 0.0f, 0.0f, 1.0f,
         -0.03f,    0.0f, 0.0f, 0.0f, 1.0f,
         -0.02f, -0.025f, 0.0f, 0.0f, 1.0f,
         0.02f, -0.025f, 0.0f, 0.0f, 1.0f,
         0.02f, -0.025f, 0.0f, 0.0f, 1.0f,
         0.05f, -0.025f, 0.0f, 0.0f, 1.0f,
         0.03f,    0.0f, 0.0f, 0.0f, 1.0f,
         -0.03f,    0.0f, 0.0f, 0.0f, 1.0f,
         0.02f, -0.025f, 0.0f, 0.0f, 1.0f,
         0.03f,    0.0f, 0.0f, 0.0f, 1.0f,
         0.03f,    0.0f, 0.0f, 0.0f, 1.0f,
         0.04f,   0.05f, 0.0f, 0.0f, 1.0f,
         0.0f,   0.01f, 0.0f, 0.0f, 1.0f,
         -0.03f,    0.0f, 0.0f, 0.0f, 1.0f,
         0.03f,    0.0f, 0.0f, 0.0f, 1.0f,
         0.0f,   0.01f, 0.0f, 0.0f, 1.0f,
         -0.03f,    0.0f, 0.0f, 0.0f, 1.0f,
         0.0f,   0.01f, 0.0f, 0.0f, 1.0f,
         -0.04f,   0.05f, 0.0f, 0.0f, 1.0f
    } };
    geometryMeshes->consume(vkMesh::GeometryType::Star, vertices);

    vkMesh::FinalizationInput finalizationInput;
    finalizationInput.device = device;
    finalizationInput.physicalDevice = physicalDevice;
    finalizationInput.queue = graphicsQueue;
    finalizationInput.commandBuffer = mainCommandBuffer;
    geometryMeshes->finalize(finalizationInput);
}

void engine::render::vulkan::RendererVulkan::prepareScene(vk::CommandBuffer commandBuffer) {
    vk::Buffer vertexBuffers[] { geometryMeshes->vertexBuffer.buffer };
    vk::DeviceSize offsets[] = { 0 };
    commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);
}
