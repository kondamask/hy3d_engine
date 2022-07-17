#ifndef INCLUDE_PLATFORM_H
#define INCLUDE_PLATFORM_H

#include "core/utils.h"
#include "core/ASSERT.h"
#include "core/logger.h"
#include "defines.h"

#define MIN_WINDOW_WIDTH 255
#define MIN_WINDOW_HEIGHT 255

struct platform_state
{
	void *data;
};
static_func bool PlatformInitialize(platform_state *platformState, const char *appName, i32 width, i32 height);
static_func bool PlatformTerminate(platform_state *platformState);
static_func bool PlatformProcessMessages(platform_state *platformState);

struct read_file_result
{
	void *content;
	u32 size;
};
static_func read_file_result PlatformReadFile(const char *filepath);
static_func bool PlatformWriteFile(const char *filepath, u32 memorySize, void *memory);
static_func void PlatformFreeFileMemory(void *memory);

struct file_write_time
{
	void *data;
};
static_func bool PlatformGetFileWriteTime(const char *filepath, file_write_time *writeTime);
static_func bool PlatformWasFileUpdated(const char *filepath, file_write_time *writeTime);

static_func void PlatformPrint(const char *message, u8 colour);
static_func void PlatformPrintError(const char *message, u8 colour);

#endif // INCLUDE_PLATFORM_H
