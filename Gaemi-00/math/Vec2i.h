//
// Created by gaetz on 23/12/2021.
//

#ifndef MATH_VEC2I_H
#define MATH_VEC2I_H

#include "../Defines.h"
#include "Functions.h"


namespace gmath {
    class Vec2;

    class Vec2i {
    public:
        /// Default constructor, return a zero vector
        Vec2i();

        /// Constructor with x and y params
        /// \param x Horizontal component
        /// \param y Vertical component
        Vec2i(i32 x, i32 y);

        /// Get the vector length
        /// \return Vector's magnitude
        f32 length() const;

        /// Get the vector squared length
        /// \return Vector's squared length
        u32 squareLength() const;

        /// Add a vector to this vector
        /// \param other Vector we want to add
        /// \return Same vector, affected by the addition
        Vec2i& operator+=(const Vec2i& other) {
            x += other.x;
            y += other.y;
            return *this;
        }

        /// Addition operator
        /// \param r Vector to add
        /// \return New vector, result of the addition
        Vec2i operator+(const Vec2i& r) {
            Vec2i sum = *this;
            sum += r;
            return sum;
        }

        /// subtract a vector to this vector
        /// \param other Vector we want to subtract
        /// \return Same vactor, affected by the subtraction
        Vec2i& operator-=(const Vec2i& other) {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        /// subtraction operator
        /// \param r Vector to subtract
        /// \return New vector, result of the subtraction
        Vec2i operator-(const Vec2i& r) {
            Vec2i sum = *this;
            sum -= r;
            return sum;
        }

        /// Equality operator
        /// \param other Tested vector
        /// \return True if vector a nearly equals
        bool operator==(const Vec2i& other) const {
            return x == other.x && y == other.y;
        }

        /// Multiply a vector by a int
        /// \param factor Factor of multiplication
        /// \return Same vector, multiplied
        Vec2i& operator*=(i32 factor) {
            x *= factor;
            y *= factor;
            return *this;
        }

        /// Multiplicator operator, with int
        /// \param factor Factor
        /// \return New vector, result of the multiplication
        Vec2i operator*(i32 factor) {
            Vec2i product = *this;
            product *= factor;
            return product;
        }

        /// Multiply a vector by a long int
        /// \param factor Factor of multiplication
        /// \return Same vector, multiplied
        Vec2i& operator*=(i64 factor) {
            x *= static_cast<float>(factor);
            y *= static_cast<float>(factor);
            return *this;
        }

        /// Multiplicator operator, with long int
        /// \param factor Factor
        /// \return New vector, result of the multiplication
        Vec2i operator*(i64 factor) {
            Vec2i product = *this;
            product *= factor;
            return product;
        }

        /// Horizontal coordinate
        i32 x{0};

        /// Vertical coordinate
        i32 y{0};

        /// Zero vector
        static Vec2i zero() { return {}; };

        /// Right vector
        static Vec2i right() { return {1, 0}; };

        /// Up vector
        static Vec2i up() { return {0, 1}; };

        /// Convert this vector of ints to a vector of float
        /// \return Corresponding Vec2
        Vec2 toVec2();
    };
}

#endif //MATH_VEC2I_H
