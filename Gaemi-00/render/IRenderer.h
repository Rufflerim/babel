//
// Created by gaetz on 26/12/2021.
//

#ifndef RENDER_IRENDERERBACKEND_H
#define RENDER_IRENDERERBACKEND_H

#include "../Window.h"
#include "../math/Vec2.h"
#include "../math/Color.h"
#include "../math/Rectangle.h"
#include "../Gaemi.h"

using gmath::Color;
using gmath::Vec2;
using gmath::Rectangle;

namespace engine::render {
    class IRenderer {
    public:
        virtual ~IRenderer() = default;

        /// Initialize renderer and set it to render in given window
        /// \param window Window in which renders happen
        /// \return True if the renderer is correctly initiliazed
        virtual bool init(Window& window) = 0;

        /// Clear the screen before drawing
        virtual void clearScreen() = 0;

        /// Organize drawing before doing it, if needed
        virtual void beginDraw() = 0;

        /// Called to draw a filled colored rectangle
        /// \param rectangle Rectangle coordinates in screen coordinates
        /// \param color Fill color of the rectangle
        GAPI virtual void drawRectangle(const Rectangle &rectangle, const Color &color) = 0;

        /// After all draw calls, organize draw if needed and present image
        virtual void endDraw() = 0;

        /// Close renderer
        virtual void close() = 0;
    };
}

#endif //RENDER_IRENDERERBACKEND_H
