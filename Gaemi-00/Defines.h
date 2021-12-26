#pragma once
#ifndef DEFINES_H
#define DEFINES_H

#include <string>
#include <bitset>
#include "../Externals/stb_image/stb_image.h"

// General macro
#define GASSERTIONS_ENABLED
#define STB_IMAGE_IMPLEMENTATION

// General types
// -- Unsigned integers
using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

// -- Signed integers
using i8 =  signed char;
using i16 = signed short;
using i32 = signed int;
using i64 = signed long long;

// -- Floating point
using f32 = float;
using f64 = double;

// -- Booleans
using b32 = int;

// -- String
using str = std::string;

// ECS
// -- Constants
constexpr u8 MAX_COMPONENTS = 32;
constexpr u32 MAX_ENTITIES = 5000;
// -- Types
using Entity = u32;
using ComponentType = u8;
using Signature = std::bitset<MAX_COMPONENTS>;


// Static assertion
#if defined(__clang__) || defined(__gcc__)
    #define STATIC_ASSERT _Static_assert
#else
    #define STATIC_ASSERT static_assert
#endif

// Ensure all types are of the correct size
STATIC_ASSERT(sizeof(u8) == 1, "Expected u8 to be 1 byte.");
STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 bytes.");
STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 bytes.");
STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 bytes.");

STATIC_ASSERT(sizeof(i8) == 1, "Expected i8 to be 1 byte.");
STATIC_ASSERT(sizeof(i16) == 2, "Expected i16 to be 2 bytes.");
STATIC_ASSERT(sizeof(i32) == 4, "Expected i32 to be 4 bytes.");
STATIC_ASSERT(sizeof(i64) == 8, "Expected i64 to be 8 bytes.");

STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 bytes.");
STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 bytes.");

constexpr std::uint32_t fnv1a_32(char const* s, std::size_t count)
{
	return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u; // NOLINT (hicpp-signed-bitwise)
}

constexpr std::uint32_t operator "" _hash(char const* s, std::size_t count)
{
	return fnv1a_32(s, count);
}

// Platform detection
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
	#define GPLATFORM_WINDOWS 1
	#ifndef _WIN64
		#error "64-bit is required on Windows!"
	#endif
#endif

// Debug
#if defined(_DEBUG) || defined(DEBUG)
	#define GDEBUG 1
#elif defined(NDEBUG)
	#define GRELEASE 1
#endif

#endif