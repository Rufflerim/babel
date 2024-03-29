//
// Created by gaetz on 25/12/2021.
//

#ifndef MATH_COLOR_H
#define MATH_COLOR_H

#include "../Defines.h"
#include <array>

namespace gmath {
    class Color {
    public:
        /// Default constructor, white
        Color() = default;

        /// Color by-component construction
        /// \param rP Red, between 0 and 255
        /// \param gP Green, between 0 and 255
        /// \param bP Blue, between 0 and 255
        /// \param aP Alpha, between 0 and 255
        Color(u8 rP, u8 gP, u8 bP, u8 aP) : r{rP}, g{gP}, b{bP}, a{aP} {}

        /// Converts a 0-255 color component to a 0-1 float
        /// \param component Color component between 0 and 255
        /// \return Color component between 0 and 1
        static float componentToFloat(u8 component) {
            return static_cast<float>(component) / 255.0f;
        }

        std::array<float, 4> toFloatArray() const {
            return { componentToFloat(r), componentToFloat(g),
                    componentToFloat(b), componentToFloat(a) };
        }

        u8 r{255}, g{255}, b{255}, a{255};

        static const Color WHITE;
        static const Color BLACK;
        static const Color RED;
        static const Color GREEN;
        static const Color BLUE;
        static const Color MAGENTA;
        static const Color YELLOW;
        static const Color CYAN;
    };
}

#endif //MATH_COLOR_H
