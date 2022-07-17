#ifndef INCLUDE_ASSERT_H
#define INCLUDE_ASSERT_H

#include "defines.h"

// Copied from https://github.com/travisvroman/kohi/commit/b9a8729ceb7b4681b1694580f96961c06ac33b6f

#define ASSERT_ENABLED

#ifdef ASSERT_ENABLED
#if _MSC_VER
#include <intrin.h>
#define HY3D_DebugBreak() __debugbreak()
#else
#define HY3D_DebugBreak() __builtin_trap()
#endif

static_func void ReportAssertionFailure(const char *expression, const char *message, const char *file, i32 line);

#define ASSERT(expr)                                               \
	{                                                              \
		if (expr)                                                  \
		{                                                          \
		}                                                          \
		else                                                       \
		{                                                          \
			ReportAssertionFailure(#expr, "", __FILE__, __LINE__); \
			HY3D_DebugBreak();                                     \
		}                                                          \
	}

#define ASSERT_MSG(expr, message)                                       \
	{                                                                   \
		if (expr)                                                       \
		{                                                               \
		}                                                               \
		else                                                            \
		{                                                               \
			ReportAssertionFailure(#expr, message, __FILE__, __LINE__); \
			HY3D_DebugBreak();                                          \
		}                                                               \
	}

#ifdef _DEBUG
#define ASSERT_DEBUG(expr)                                         \
	{                                                              \
		if (expr)                                                  \
		{                                                          \
		}                                                          \
		else                                                       \
		{                                                          \
			ReportAssertionFailure(#expr, "", __FILE__, __LINE__); \
			HY3D_DebugBreak();                                     \
		}                                                          \
	}
#else
#define ASSERT_DEBUG(expr) // Does nothing at all
#endif

#else
#define ASSERT(expr)			  // Does nothing at all
#define ASSERT_MSG(expr, message) // Does nothing at all
#define ASSERT_DEBUG(expr)		  // Does nothing at all
#endif

#endif // INCLUDE_ASSERT_H