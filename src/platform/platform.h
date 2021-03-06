#ifndef INCLUDE_PLATFORM_H
#define INCLUDE_PLATFORM_H

#include "core/utils.h"
#include "core/ASSERT.h"
#include "core/logger.h"
#include "core/input.h"
#include "defines.h"

namespace HY3D
{
#define MIN_WINDOW_WIDTH 255
#define MIN_WINDOW_HEIGHT 255

	struct platform_state
	{
		void *data;
	};
	bool PlatformInitialize(platform_state *platformState, const char *appName, i32 width, i32 height);
	bool PlatformTerminate(platform_state *platformState);
	bool PlatformProcessMessages(platform_state *platformState);

	struct read_file_result
	{
		void *content;
		u32 size;
	};
	read_file_result PlatformReadFile(const char *filepath);
	bool PlatformWriteFile(const char *filepath, u32 memorySize, void *memory);
	void PlatformFreeFileMemory(void *memory);

	struct file_write_time
	{
		void *data;
	};
	bool PlatformGetFileWriteTime(const char *filepath, file_write_time *writeTime);
	bool PlatformWasFileUpdated(const char *filepath, file_write_time *writeTime);

	void PlatformPrint(const char *message, u8 colour);
	void PlatformPrintError(const char *message, u8 colour);

	struct dynamic_library
	{
		char name[255];
		file_write_time writeTime;
		void *data; // ptr to os specific data
	};
	bool PlatformLoadDynamicLibrary(const char *filepath, dynamic_library *libOut);
	void *PlatformGetDynamicLibraryFunction(dynamic_library *lib, const char *function);
	bool PlatformUnloadDynamicLibrary(dynamic_library *lib);
	bool PlatformUpdatedDynamicLibrary(dynamic_library *lib);
	bool PlatformReloadDynamicLibrary(dynamic_library *lib);
}
#endif // INCLUDE_PLATFORM_H
