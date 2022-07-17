#ifndef INCLUDE_DEFINES_H
#define INCLUDE_DEFINES_H

typedef char i8;
typedef short i16;
typedef int i32;
typedef long i64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;
typedef float f32;
typedef double f64;

#define global_var static
#define local_var static
#define static_func static

// Platform detection
// Copied from: https://github.com/travisvroman/kohi/commit/06fcdd94c82d5871bae6a2883850f53138b85f20#diff-cf715656a36e808563d22936ef0ba650eac1a565d15f8406eb813d6b6f0b26df
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define PLATFORM_WINDOWS 1
#ifndef _WIN64
#error "64-bit is required on Windows!"
#endif
#elif defined(__linux__) || defined(__gnu_linux__)
// Linux OS
#define PLATFORM_LINUX 1
#if defined(__ANDROID__)
#define PLATFORM_ANDROID 1
#endif
#elif defined(__unix__)
// Catch anything not caught by the above.
#define PLATFORM_UNIX 1
#elif defined(_POSIX_VERSION)
// Posix
#define PLATFORM_POSIX 1
#elif __APPLE__
// Apple platforms
#define PLATFORM_APPLE 1
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
// iOS Simulator
#define PLATFORM_IOS 1
#define PLATFORM_IOS_SIMULATOR 1
#elif TARGET_OS_IPHONE
#define PLATFORM_IOS 1
// iOS device
#elif TARGET_OS_MAC
// Other kinds of Mac OS
#else
#error "Unknown Apple platform"
#endif
#else
#error "Unknown platform!"
#endif

#define HY3D_EXPORT
#ifdef HY3D_EXPORT
// Exports
#ifdef _MSC_VER
#define HY3D_API __declspec(dllexport)
#else
#define HY3D_API __attribute__((visibility("default")))
#endif
#else
// Imports
#ifdef _MSC_VER
#define HY3D_API __declspec(dllimport)
#else
#define HY3D_API
#endif
#endif

#endif // INCLUDE_DEFINES_H
