//
// Created by gaetz on 23/12/2021.
//

#ifndef MATH_VEC2_H
#define MATH_VEC2_H

#include "../Defines.h"
#include "Functions.h"


namespace gmath {
    constexpr f32 VEC2_FLOAT_EPSILON = 0.000001f;
    class Vec2i;

    class Vec2 {
    public:
        /// Default constructor, return a zero vector
        Vec2();

        /// Constructor with x and y params
        /// \param x Horizontal component
        /// \param y Vertical component
        Vec2(f32 x, f32 y);

        /// Get the vector length
        /// \return Vector's magnitude
        f32 length() const;

        /// Get the vector squared length
        /// \return Vector's squared length
        f32 squareLength() const;

        /// Add a vector to this vector
        /// \param other Vector we want to add
        /// \return Same vactor, affected by the addition
        Vec2& operator+=(const Vec2& other) {
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

        /// Substract a vector to this vector
        /// \param other Vector we want to substract
        /// \return Same vactor, affected by the substraction
        Vec2& operator-=(const Vec2& other) {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        /// Substraction operator
        /// \param r Vector to substract
        /// \return New vector, result of the substraction
        Vec2 operator-(const Vec2& r) {
            Vec2 sum = *this;
            sum -= r;
            return sum;
        }

        /// Equality operator
        /// \param other Tested vector
        /// \return True if vector a nearly equals
        bool operator==(const Vec2& other) const {
            Vec2 diff { x - other.x, y - other.y };
            return gmath::nearZero(diff.x, VEC2_FLOAT_EPSILON) && gmath::nearZero(diff.y, VEC2_FLOAT_EPSILON);
        }

        /// Multiply a vector by a float
        /// \param factor Factor of multiplication
        /// \return Same vector, multiplied
        Vec2& operator*=(f32 factor) {
            x *= factor;
            y *= factor;
            return *this;
        }

        /// Multiplicator operator, with float
        /// \param factor Factor
        /// \return New vector, result of the multiplication
        Vec2 operator*(f32 factor) {
            Vec2 product = *this;
            product *= factor;
            return product;
        }

        /// Multiply a vector by a double
        /// \param factor Factor of multiplication
        /// \return Same vector, multiplied
        Vec2& operator*=(f64 factor) {
            x *= static_cast<float>(factor);
            y *= static_cast<float>(factor);
            return *this;
        }

        /// Multiplicator operator, with double
        /// \param factor Factor
        /// \return New vector, result of the multiplication
        Vec2 operator*(f64 factor) {
            Vec2 product = *this;
            product *= factor;
            return product;
        }

        /// Normalize this vector
        /// \return This vector, normalized
        Vec2& normalize();

        /// Convert this vector of ints to a vector of int
        /// \return Corresponding Vec2i
        Vec2i toVec2i();

        /// Horizontal coordinate
        f32 x{0.0f};

        /// Vertical coordinate
        f32 y{0.0f};

        /// Zero vector
        static Vec2 zero() { return {}; }

        /// Right vector
        static Vec2 right() { return {1, 0}; }

        /// Up vector
        static Vec2 up() { return {0, 1}; }
    };
}

#endif //MATH_VEC2_H
