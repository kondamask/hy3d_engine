#if !defined(INCLUDE_WIN32_PLATFORM_H)
#define INCLUDE_WIN32_PLATFORM_H

#include "hy3d_windows.h"
#include "resource.h"

#include "defines.h"

#define WIN32_WINDOW_Y_BORDER 39
#define WIN32_WINDOW_X_BORDER 23

#define VK_CODE unsigned char

struct win32_window
{
	u32 width, height;
	HWND handle;
	HINSTANCE instance;
	const char *name = "hy3d vulkan";
};
typedef win32_window os_window_context;
global_var os_window_context osWindowContext;

struct win32_engine_code
{
	HMODULE dll;
	FILETIME writeTime;

	bool isValid;
};

#endif // INCLUDE_WIN32_PLATFORM_H
