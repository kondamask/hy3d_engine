#include "assert.h"
#include "logger.h"

namespace HY3D
{
	void ReportAssertionFailure(const char *expression, const char *message, const char *file, i32 line)
	{
		LogOutput(LOG_LEVEL_FATAL, "%s(%d): Assertion Failed: %s, Message: %s\n", file, line, expression, message);
	}
}