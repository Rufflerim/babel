#pragma once
#ifndef DEFINES_H
#define DEFINES_H

#include <string>

// General macro
#define GASSERTIONS_ENABLED


// Unsigned integers
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

// Signed integers
typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

// Floating point
typedef float f32;
typedef double f64;

// Booleans
typedef int b32;

// String
typedef std::string str;

#define STATIC_ASSERT static_assert

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

// Platform detection
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
	#define GPLATFORM_WINDOWS 1
	#ifndef _WIN64
		#error "64-bit is required on Windows!"
	#endif
#endif

// Debug
#ifdef _DEBUG
	#define GDEBUG 1
#elif defined(NDEBUG)
	#define GRELEASE 1
#endif

#endif