//
// Created by gaetz on 26/12/2021.
//

#ifndef RENDER_IRENDERERBACKEND_H
#define RENDER_IRENDERERBACKEND_H

#include "../ILocator.h"
#include "../Window.h"
#include "../math/Color.h"

namespace gmath {
    class Vec2;
    class Rectangle;
    class RectangleInt;
}

namespace engine {

    namespace render {

    enum class Flip {
        None = 0,
        Horizontal = 1,
        Vertical = 2,
        HorizontalAndVertical = 3
    };

    class IRenderer {
    public:
        virtual ~IRenderer() = default;

        /// Initialize renderer and set it to render in given window
        /// \param window Window in which renders happen
        /// \return True if the renderer is correctly initiliazed
        virtual bool init(ILocator* locator, Window& window) = 0;

        /// Clear the screen before drawing
        virtual void clearScreen() = 0;

        /// Organize drawing before doing it, if needed
        virtual void beginDraw() = 0;

        /// After all draw calls, organize draw if needed and present image
        virtual void endDraw() = 0;

        /// Close renderer
        virtual void close() = 0;

        /// Called to draw a filled colored rectangle
        /// \param rectangle Rectangle coordinates in screen coordinates
        /// \param color Fill color of the rectangle
        GAPI virtual void drawRectangle(const gmath::Rectangle &rectangle, const gmath::Color &color) = 0;

        GAPI virtual void drawSprite(sdl::Texture* texture,
                                     const gmath::RectangleInt& srcRect,
                                     const gmath::RectangleInt& dstRect,
                                     f64 angle,
                                     const gmath::Vec2& origin,
                                     Flip flip = Flip::None) = 0;
    };
} }

#endif //RENDER_IRENDERERBACKEND_H
