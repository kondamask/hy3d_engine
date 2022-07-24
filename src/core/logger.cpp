#include "logger.h"
#include "platform/platform.h"

#include <stdarg.h>

namespace HY3D
{
#include <stdio.h>
#include <string.h>

	namespace Logger
	{
		global_var logger_api API;
	}
	void LogOutput(log_level level, const char *message, ...)
	{
		const char *level_strings[6] = {"[FATAL]: ", "[ERROR]: ", "[WARN]:  ", "[INFO]:  ", "[DEBUG]: ", "[TRACE]: "};
		bool is_error = level < LOG_LEVEL_WARN;

		// Technically imposes a 32k character limit on a single log entry, but...
		// DON'T DO THAT!
		const i32 msg_length = 32000;
		char out_message[msg_length];
		memset(out_message, 0, sizeof(out_message));

		// Format original message.
		// NOTE: Oddly enough, MS's headers override the GCC/Clang va_list type with a "typedef char* va_list" in some
		// cases, and as a result throws a strange error here. The workaround for now is to just use __builtin_va_list,
		// which is the type GCC/Clang's va_start expects.
		va_list arg_ptr;
		va_start(arg_ptr, message);
		vsnprintf(out_message, msg_length, message, arg_ptr);
		va_end(arg_ptr);

		char out_message2[msg_length];
		sprintf(out_message2, "%s%s\n", level_strings[level], out_message);

#if 1
		if (is_error)
			Logger::API.PrintError(out_message2, (u8)level);
		else
			Logger::API.Print(out_message2, (u8)level);
#else
		printf(out_message2);
#endif
	}
}
