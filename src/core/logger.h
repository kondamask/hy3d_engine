#ifndef INCLUDE_LOGGER_H
#define INCLUDE_LOGGER_H

#include "defines.h"

namespace HY3D
{
// Copied from: https://github.com/travisvroman/kohi/commit/b9a8729ceb7b4681b1694580f96961c06ac33b6f
#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1

// Disable debug and trace logging for release builds.
#if HY3D_RELEASE == 1
#define LOG_DEBUG_ENABLED 0
#endif
	enum log_level
	{
		LOG_LEVEL_ERROR,
		LOG_LEVEL_WARN,
		LOG_LEVEL_INFO,
		LOG_LEVEL_DEBUG
	};

	struct logger_api
	{
		void (*Print)(const char *message, u8 colour);
		void (*PrintError)(const char *message, u8 colour);
	};

	void LogOutput(log_level level, const char *message, ...);

#define LOG_ERROR(message, ...) LogOutput(LOG_LEVEL_ERROR, message, ##__VA_ARGS__)

#if LOG_WARN_ENABLED == 1
// Logs a warning-level message.
#define LOG_WARN(message, ...) LogOutput(LOG_LEVEL_WARN, message, ##__VA_ARGS__)
#else
// Does nothing when LOG_WARN_ENABLED != 1
#define LOG_WARN(message, ...)
#endif

#if LOG_INFO_ENABLED == 1
// Logs a info-level message.
#define LOG_INFO(message, ...) LogOutput(LOG_LEVEL_INFO, message, ##__VA_ARGS__)
#else
// Does nothing when LOG_INFO_ENABLED != 1
#define LOG_INFO(message, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
// Logs a debug-level message.
#define LOG_DEBUG(message, ...) LogOutput(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
#else
// Does nothing when LOG_DEBUG_ENABLED != 1
#define LOG_DEBUG(message, ...)
#endif
}
#endif // INCLUDE_LOGGER_H