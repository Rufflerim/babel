//
// Created by gaetz on 03/10/2022.
//

#ifndef RENDER_VULKAN_RENDERERVULKAN_H
#define RENDER_VULKAN_RENDERERVULKAN_H


#include "../IRenderer.h"
#include "../Texture.h"
#include <vulkan/vulkan.hpp>

namespace engine::render::vulkan {
    class RendererVulkan : public IRenderer {
    public:
        RendererVulkan() = default;

        bool init(engine::ILocator* locator, IWindow& window) override;

        void clearScreen() override;

        void beginDraw() override;

        void drawRectangle(const gmath::Rectangle& rectangle, const gmath::Color& color) override;

        void endDraw() override;

        void close() override;

        void drawSprite(Texture* texture, const gmath::RectangleInt& srcRect, const gmath::RectangleInt& dstRect,
                        f64 angle, const gmath::Vec2& origin, engine::render::Flip flip) override;

    private:
        ILocator* locator{nullptr};
        gmath::Color clearColor{gmath::Color::BLACK};
        i32 width{-1};
        i32 height{-1};

        vk::Instance instance;
    };
};


#endif //BABEL_RENDERERVULKAN_H
