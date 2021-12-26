//
// Created by gaetz on 23/12/2021.
//

#ifndef MATH_VEC2_H
#define MATH_VEC2_H

#include "../Defines.h"

namespace gmath {
    class Vec2 {
    public:
        /// Default constructor, return a zero vector
        Vec2();

        /// Constructor with x and y params
        /// \param x Horizontal component
        /// \param y Vertical component
        Vec2(float x, float y);

        /// Get the vector length
        /// \return Vector's magnitude
        f32 length() const;

        /// Get the vector squared length
        /// \return Vector's squared length
        f32 squareLength() const;

        /// Multiply a vector by a float
        /// \param factor Factor of multiplication
        /// \return Same vector, multiplied
        Vec2& operator*=(f32 factor) {
            x *= factor;
            y *= factor;
            return *this;
        }

        /// Add a vector to this vector
        /// \param other Vector we want to add
        /// \return Same vactor, affected by the addition
        Vec2& operator+=(Vec2 other) {
            x += other.x;
            y += other.y;
            return *this;
        }

        /// Addition operator
        /// \param r Vector to add
        /// \return New vector, result of the addition
        Vec2 operator+(const Vec2& r) {
            Vec2 sum = *this;
            sum += r;
            return sum;
        }

        /// Normalize this vector
        /// \return This vector, normalized
        Vec2& normalize();

        /// Horizontal coordinate
        f32 x{0.0f};

        /// Vertical coordinate
        f32 y{0.0f};
    };
}

#endif //MATH_VEC2_H
