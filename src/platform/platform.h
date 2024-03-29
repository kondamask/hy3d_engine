#ifndef INCLUDE_PLATFORM_H
#define INCLUDE_PLATFORM_H

#include "defines.h"

namespace HY3D
{
#define MIN_WINDOW_WIDTH 255
#define MIN_WINDOW_HEIGHT 255

	struct platform_state
	{
		void* data;
	};

	const char* PlatformGetAppName(platform_state *platformState);
	bool PlatformSetWindowTitle(platform_state* platformState, const char *title);

	bool PlatformInitialize(platform_state* platformState, const char* appName, i32 width, i32 height);
	bool PlatformTerminate(platform_state* platformState);
	bool PlatformProcessMessages(platform_state* platformState);

	struct read_file_result
	{
		void* content;
		u32 size;
	};
	read_file_result PlatformReadFile(const char* filepath);
	bool PlatformWriteFile(const char* filepath, u32 memorySize, void* memory);
	void PlatformFreeFileMemory(void* memory);

	struct file_write_time
	{
		void* data;
	};
	bool PlatformGetFileWriteTime(const char* filepath, file_write_time* writeTime);
	bool PlatformWasFileUpdated(const char* filepath, file_write_time* writeTime);

	void PlatformPrint(const char* message, u8 colour);
	void PlatformPrintError(const char* message, u8 colour);

	f64 PlatformGetTime();
	void PlatformSleep(u64 ms);

	struct dynamic_library
	{
		char name[255];
		file_write_time writeTime;
		void* data; // ptr to os specific data
	};

	bool PlatformLoadLibrary(const char* filepath, dynamic_library* libOut);
	bool PlatformLoadSystemLibrary(const char* filepath, dynamic_library* libOut);
	bool PlatformUnloadLibrary(dynamic_library* lib);
	void* PlatformGetLibraryFunction(dynamic_library* lib, const char* function);
	bool PlatformUpdatedDynamicLibrary(dynamic_library* lib);

	void PlatformCreateVulkanSurface(platform_state* platformState, void* surfaceInfoIn);
}
#endif // INCLUDE_PLATFORM_H
