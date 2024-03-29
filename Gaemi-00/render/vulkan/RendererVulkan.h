//
// Created by gaetz on 03/10/2022.
//

#ifndef RENDER_VULKAN_RENDERERVULKAN_H
#define RENDER_VULKAN_RENDERERVULKAN_H

#define VULKAN_HPP_NO_EXCEPTIONS = 1

#include "../IRenderer.h"
#include "../Texture.h"
#include "Frame.h"
#include "TestScene.h"
#include "VertexBufferAtlas.h"

namespace engine::render::vulkan {
    class RendererVulkan : public IRenderer {
    public:
        RendererVulkan() = default;

        bool init(engine::ILocator* locator, IWindow& window) override;

        void load() override;

        void clearScreen() override;

        void beginDraw() override;

        void endDraw() override;

        void close() override;

        void drawRectangle(const gmath::Rectangle& rectangle, const gmath::Color& color) override;

        void drawSprite(Texture* texture, const gmath::RectangleInt& srcRect, const gmath::RectangleInt& dstRect,
                        f64 angle, const gmath::Vec2& origin, engine::render::Flip flip) override;

    private:
        ILocator* locator { nullptr };
        gmath::Color clearColor { gmath::Color::BLACK };
        i32 width { -1 };
        i32 height { -1 };

        TestScene testScene {};
        TextureGPU testTexture {};

        vk::Instance instance;
        vk::DebugUtilsMessengerEXT debugMessenger { nullptr };
        vk::DispatchLoaderDynamic dynamicInstanceLoader;
        vk::PhysicalDevice physicalDevice { nullptr };
        vk::Device device { nullptr };
        vk::Queue graphicsQueue { nullptr };
        vk::Queue presentQueue { nullptr };
        vk::SurfaceKHR surface;

        vk::SwapchainKHR swapchain;
        vector<vkUtils::SwapchainFrame> swapchainFrames;
        vk::Format swapchainFormat;
        vk::Extent2D swapchainExtent;

        vk::DescriptorSetLayout descriptorSetLayout;
        vk::DescriptorPool descriptorPool;

        vk::PipelineLayout layout;
        vk::RenderPass renderPass;
        vk::Pipeline pipeline;

        vk::CommandPool commandPool;
        vk::CommandBuffer mainCommandBuffer;
        i32 maxFrameInFlight;
        i32 currentFrameNumber;

        vkMesh::VertexBufferAtlas* geometryMeshes;

        void makeSwapchain();
        void closeSwapchain();
        void recreateSwapchain();
        void makeFramebuffers();
        void makeSyncObjects();
        vk::DescriptorPool createDescriptorPool();

        void makeAssets();
        void makeTextures();
        void prepareScene(vk::CommandBuffer commandBuffer, u32 imageIndex);

        void recordDrawCommands(vk::CommandBuffer commandBuffer, u32 imageIndex, TestScene& scene);
        void render(TestScene& testScene);
    };
};


#endif //RENDER_VULKAN_RENDERERVULKAN_H
