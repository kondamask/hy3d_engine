#include "platform/platform.h"

#if PLATFORM_WINDOWS

namespace HY3D
{
#include "resource.h"
#include "hy3d_windows.h"
#include <stdio.h>

#define WIN32_WINDOW_X_BORDER 23
#define WIN32_WINDOW_Y_BORDER 39

	struct win32_platform_state
	{
		HWND handle;
		HINSTANCE instance;
		i32 width;
		i32 height;
		const char *name;
	};

	struct win32_dll
	{
		HMODULE dll;
	};

	// Windows VK_CODES: https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
	namespace Win32VkCode
	{
		global_var input_button toInputButton[] = {
			INPUT_BUTTON_UNSUPPORTED,
			MOUSE_BUTTON_LEFT,		   // VK_LBUTTON 	    0x1 	The left mouse button
			MOUSE_BUTTON_RIGHT,		   // VK_RBUTTON 	    0x2 	The right mouse button
			INPUT_BUTTON_UNSUPPORTED,  // VK_CANCEL 	    0x3 	The Cancel virtual key, used for control-break processing
			MOUSE_BUTTON_MIDDLE,	   // VK_MBUTTON 	    0x4 	The middle mouse button
			INPUT_BUTTON_UNSUPPORTED,  // VK_XBUTTON1 		0x5 	X1 mouse button
			INPUT_BUTTON_UNSUPPORTED,  // VK_XBUTTON2		0x6 	X2 mouse button
			INPUT_BUTTON_UNSUPPORTED,  // -					0x7 	Undefined
			KEYBOARD_BUTTON_BACKSPACE, // VK_BACK 		    0x8 	Backspace
			KEYBOARD_BUTTON_TAB,	   // VK_TAB 		    0x9 	Tab
			INPUT_BUTTON_UNSUPPORTED,  // -		    		0xA 	Reserved
			INPUT_BUTTON_UNSUPPORTED,  // -		    		0xB 	Reserved
			INPUT_BUTTON_UNSUPPORTED,  // VK_CLEAR 	    	0xC 	5 (keypad without Num Lock)
			KEYBOARD_BUTTON_ENTER,	   // VK_RETURN 	    0xD 	Enter
			INPUT_BUTTON_UNSUPPORTED,  // - 	    		0xE 	Undefined
			INPUT_BUTTON_UNSUPPORTED,  // - 	    		0xF 	Undefined
			KEYBOARD_BUTTON_SHIFT,	   // VK_SHIFT 	    	0x10 	Shift (either one)
			KEYBOARD_BUTTON_CTRL,	   // VK_CONTROL 	    0x11 	Ctrl (either one)
			KEYBOARD_BUTTON_ALT,	   // VK_MENU 		    0x12 	Alt (either one)
			INPUT_BUTTON_UNSUPPORTED,  // VK_PAUSE 	    	0x13 	Pause
			KEYBOARD_BUTTON_CAPS,	   // VK_CAPITAL 	    0x14 	Caps Lock
			INPUT_BUTTON_UNSUPPORTED,  // VK_HANGUEL		0x15 	IME Hanguel mode (maintained for compatibility; use VK_HANGUL)
			INPUT_BUTTON_UNSUPPORTED,  // VK_IME_ON		 	0x16 	IME On
			INPUT_BUTTON_UNSUPPORTED,  // VK_JUNJA		 	0x17 	IME Junja mode
			INPUT_BUTTON_UNSUPPORTED,  // VK_FINAL		 	0x18 	IME final mode
			INPUT_BUTTON_UNSUPPORTED,  // VK_HANJA		 	0x19 	IME Hanja mode
			INPUT_BUTTON_UNSUPPORTED,  // VK_IME_OFF		0x1A 	IME Off
			KEYBOARD_BUTTON_ESC,	   // VK_ESCAPE 	    0x1B 	Esc
			INPUT_BUTTON_UNSUPPORTED,  // VK_CONVERT	 	0x1C 	IME convert
			INPUT_BUTTON_UNSUPPORTED,  // VK_NONCONVERT	 	0x1D 	IME nonconvert
			INPUT_BUTTON_UNSUPPORTED,  // VK_ACCEPT	 		0x1E 	IME accept
			INPUT_BUTTON_UNSUPPORTED,  // VK_MODECHANGE	 	0x1F 	IME mode change request
			KEYBOARD_BUTTON_SPACE,	   // VK_SPACE 	    	0x20 	Spacebar
			INPUT_BUTTON_UNSUPPORTED,  // VK_PRIOR 	    	0x21 	Page Up
			INPUT_BUTTON_UNSUPPORTED,  // VK_NEXT 		    0x22 	Page Down
			INPUT_BUTTON_UNSUPPORTED,  // VK_END 		    0x23 	End
			INPUT_BUTTON_UNSUPPORTED,  // VK_HOME 		    0x24 	Home
			KEYBOARD_BUTTON_LEFT,	   // VK_LEFT 		    0x25 	Left Arrow
			KEYBOARD_BUTTON_UP,		   // VK_UP 		    0x26 	Up Arrow
			KEYBOARD_BUTTON_RIGHT,	   // VK_RIGHT 	    	0x27 	Right Arrow
			KEYBOARD_BUTTON_DOWN,	   // VK_DOWN 		    0x28 	Down Arrow
			INPUT_BUTTON_UNSUPPORTED,  // VK_SELECT 	    0x29 	Select
			INPUT_BUTTON_UNSUPPORTED,  // VK_PRINT 	    	0x2A 	Print (only used by Nokia keyboards)
			INPUT_BUTTON_UNSUPPORTED,  // VK_EXECUTE 	    0x2B 	Execute (not used)
			INPUT_BUTTON_UNSUPPORTED,  // VK_SNAPSHOT 	    0x2C 	Print Screen
			INPUT_BUTTON_UNSUPPORTED,  // VK_INSERT 	    0x2D 	Insert
			INPUT_BUTTON_UNSUPPORTED,  // VK_DELETE 	    0x2E 	Delete
			INPUT_BUTTON_UNSUPPORTED,  // VK_HELP 		    0x2F 	Help
			KEYBOARD_BUTTON_0,		   // VK_0 		    	0x30 	0
			KEYBOARD_BUTTON_1,		   // VK_1 		    	0x31 	1
			KEYBOARD_BUTTON_2,		   // VK_2 		    	0x32 	2
			KEYBOARD_BUTTON_3,		   // VK_3 		    	0x33 	3
			KEYBOARD_BUTTON_4,		   // VK_4 		    	0x34 	4
			KEYBOARD_BUTTON_5,		   // VK_5 		    	0x35 	5
			KEYBOARD_BUTTON_6,		   // VK_6 		    	0x36 	6
			KEYBOARD_BUTTON_7,		   // VK_7 		    	0x37 	7
			KEYBOARD_BUTTON_8,		   // VK_8 		    	0x38 	8
			KEYBOARD_BUTTON_9,		   // VK_9 		    	0x39 	9
			INPUT_BUTTON_UNSUPPORTED,  // -					0x3A 	Undefined
			INPUT_BUTTON_UNSUPPORTED,  // -					0x3B 	Undefined
			INPUT_BUTTON_UNSUPPORTED,  // -					0x3C 	Undefined
			INPUT_BUTTON_UNSUPPORTED,  // -					0x3D 	Undefined
			INPUT_BUTTON_UNSUPPORTED,  // -					0x3E 	Undefined
			INPUT_BUTTON_UNSUPPORTED,  // -					0x3F 	Undefined
			INPUT_BUTTON_UNSUPPORTED,  // -					0x40 	Undefined
			KEYBOARD_BUTTON_A,		   // VK_A 		    	0x41 	A
			KEYBOARD_BUTTON_B,		   // VK_B 		    	0x42 	B
			KEYBOARD_BUTTON_C,		   // VK_C 		    	0x43 	C
			KEYBOARD_BUTTON_D,		   // VK_D 		    	0x44 	D
			KEYBOARD_BUTTON_E,		   // VK_E 		    	0x45 	E
			KEYBOARD_BUTTON_F,		   // VK_F 		    	0x46 	F
			KEYBOARD_BUTTON_G,		   // VK_G 		    	0x47 	G
			KEYBOARD_BUTTON_H,		   // VK_H 		    	0x48 	H
			KEYBOARD_BUTTON_I,		   // VK_I 		    	0x49 	I
			KEYBOARD_BUTTON_J,		   // VK_J 		    	0x4A 	J
			KEYBOARD_BUTTON_K,		   // VK_K 		    	0x4B 	K
			KEYBOARD_BUTTON_L,		   // VK_L 		    	0x4C 	L
			KEYBOARD_BUTTON_M,		   // VK_M 		    	0x4D 	M
			KEYBOARD_BUTTON_N,		   // VK_N 		    	0x4E 	N
			KEYBOARD_BUTTON_O,		   // VK_O 		    	0x4F 	O
			KEYBOARD_BUTTON_P,		   // VK_P 		    	0x50 	P
			KEYBOARD_BUTTON_Q,		   // VK_Q 		    	0x51 	Q
			KEYBOARD_BUTTON_R,		   // VK_R 		    	0x52 	R
			KEYBOARD_BUTTON_S,		   // VK_S 		    	0x53 	S
			KEYBOARD_BUTTON_T,		   // VK_T 		    	0x54 	T
			KEYBOARD_BUTTON_U,		   // VK_U 		    	0x55 	U
			KEYBOARD_BUTTON_V,		   // VK_V 		    	0x56 	V
			KEYBOARD_BUTTON_W,		   // VK_W 		    	0x57 	W
			KEYBOARD_BUTTON_X,		   // VK_X 		    	0x58 	X
			KEYBOARD_BUTTON_Y,		   // VK_Y 		    	0x59 	Y
			KEYBOARD_BUTTON_Z,		   // VK_Z 		    	0x5A 	Z
			INPUT_BUTTON_UNSUPPORTED,  // VK_STARTKEY 	    0x5B 	Start Menu key
			INPUT_BUTTON_UNSUPPORTED,  // VK_RWIN 			0x5C 	Right Windows key (Natural keyboard)
			INPUT_BUTTON_UNSUPPORTED,  // VK_CONTEXTKEY    	0x5D 	Context Menu key
			INPUT_BUTTON_UNSUPPORTED,  // -			    	0x5E 	Reserved
			INPUT_BUTTON_UNSUPPORTED,  // VK_SLEEP 			0x5F 	Computer Sleep key
			KEYBOARD_BUTTON_NUMPAD_0,  // VK_NUMPAD0 	    0x60 	0 (keypad with Num Lock)
			KEYBOARD_BUTTON_NUMPAD_1,  // VK_NUMPAD1 	    0x61 	1 (keypad with Num Lock)
			KEYBOARD_BUTTON_NUMPAD_2,  // VK_NUMPAD2 	    0x62 	2 (keypad with Num Lock)
			KEYBOARD_BUTTON_NUMPAD_3,  // VK_NUMPAD3 	    0x63 	3 (keypad with Num Lock)
			KEYBOARD_BUTTON_NUMPAD_4,  // VK_NUMPAD4 	    0x64 	4 (keypad with Num Lock)
			KEYBOARD_BUTTON_NUMPAD_5,  // VK_NUMPAD5 	    0x65 	5 (keypad with Num Lock)
			KEYBOARD_BUTTON_NUMPAD_6,  // VK_NUMPAD6 	    0x66 	6 (keypad with Num Lock)
			KEYBOARD_BUTTON_NUMPAD_7,  // VK_NUMPAD7 	    0x67 	7 (keypad with Num Lock)
			KEYBOARD_BUTTON_NUMPAD_8,  // VK_NUMPAD8 	    0x68 	8 (keypad with Num Lock)
			KEYBOARD_BUTTON_NUMPAD_9,  // VK_NUMPAD9 	    0x69 	9 (keypad with Num Lock)
			INPUT_BUTTON_UNSUPPORTED,  // VK_MULTIPLY 	    0x6A 	* (keypad)
			INPUT_BUTTON_UNSUPPORTED,  // VK_ADD 		    0x6B 	+ (keypad)
			INPUT_BUTTON_UNSUPPORTED,  // VK_SEPARATOR     	0x6C 	Separator (never generated by the keyboard)
			INPUT_BUTTON_UNSUPPORTED,  // VK_SUBTRACT 	    0x6D 	- (keypad)
			INPUT_BUTTON_UNSUPPORTED,  // VK_DECIMAL 	    0x6E 	. (keypad with Num Lock)
			INPUT_BUTTON_UNSUPPORTED,  // VK_DIVIDE 	    0x6F 	/ (keypad)
			KEYBOARD_BUTTON_F1,		   // VK_F1 		    0x70 	F1
			KEYBOARD_BUTTON_F2,		   // VK_F2 		    0x71 	F2
			KEYBOARD_BUTTON_F3,		   // VK_F3 		    0x72 	F3
			KEYBOARD_BUTTON_F4,		   // VK_F4 		    0x73 	F4
			KEYBOARD_BUTTON_F5,		   // VK_F5 		    0x74 	F5
			KEYBOARD_BUTTON_F6,		   // VK_F6 		    0x75 	F6
			KEYBOARD_BUTTON_F7,		   // VK_F7 		    0x76 	F7
			KEYBOARD_BUTTON_F8,		   // VK_F8 		    0x77 	F8
			KEYBOARD_BUTTON_F9,		   // VK_F9 		    0x78 	F9
			KEYBOARD_BUTTON_F10,	   // VK_F10 		    0x79 	F10
			KEYBOARD_BUTTON_F11,	   // VK_F11 		    0x7A 	F11
			KEYBOARD_BUTTON_F12,	   // VK_F12 		    0x7B 	F12
			INPUT_BUTTON_UNSUPPORTED,  // VK_F13 		    0x7C 	F13
			INPUT_BUTTON_UNSUPPORTED,  // VK_F14 		    0x7D 	F14
			INPUT_BUTTON_UNSUPPORTED,  // VK_F15 		    0x7E 	F15
			INPUT_BUTTON_UNSUPPORTED,  // VK_F16 		    0x7F 	F16
			INPUT_BUTTON_UNSUPPORTED,  // VK_F17 		    0x80 	F17
			INPUT_BUTTON_UNSUPPORTED,  // VK_F18 		    0x81 	F18
			INPUT_BUTTON_UNSUPPORTED,  // VK_F19 		    0x82 	F19
			INPUT_BUTTON_UNSUPPORTED,  // VK_F20 		    0x83 	F20
			INPUT_BUTTON_UNSUPPORTED,  // VK_F21 		    0x84 	F21
			INPUT_BUTTON_UNSUPPORTED,  // VK_F22 		    0x85 	F22
			INPUT_BUTTON_UNSUPPORTED,  // VK_F23 		    0x86 	F23
			INPUT_BUTTON_UNSUPPORTED,  // VK_F24 		    0x87 	F24
			INPUT_BUTTON_UNSUPPORTED,  // -		 		    0x88 	Unassigned
			INPUT_BUTTON_UNSUPPORTED,  // -		 		    0x89 	Unassigned
			INPUT_BUTTON_UNSUPPORTED,  // -		 		    0x8A 	Unassigned
			INPUT_BUTTON_UNSUPPORTED,  // -		 		    0x8B 	Unassigned
			INPUT_BUTTON_UNSUPPORTED,  // -		 		    0x8C 	Unassigned
			INPUT_BUTTON_UNSUPPORTED,  // -		 		    0x8D 	Unassigned
			INPUT_BUTTON_UNSUPPORTED,  // -		 		    0x8E 	Unassigned
			INPUT_BUTTON_UNSUPPORTED,  // -		 		    0x8F 	Unassigned
			INPUT_BUTTON_UNSUPPORTED,  // VK_NUMLOCK 	    0x90 	Num Lock
			INPUT_BUTTON_UNSUPPORTED,  // VK_OEM_SCROLL    	0x91 	Scroll Lock
			INPUT_BUTTON_UNSUPPORTED,  // -			    	0x92 	OEM specific
			INPUT_BUTTON_UNSUPPORTED,  // -			    	0x93 	OEM specific
			INPUT_BUTTON_UNSUPPORTED,  // -			    	0x94 	OEM specific
			INPUT_BUTTON_UNSUPPORTED,  // -			    	0x95 	OEM specific
			INPUT_BUTTON_UNSUPPORTED,  // -			    	0x96 	OEM specific
			INPUT_BUTTON_UNSUPPORTED,  // -			    	0x97 	Unassigned
			INPUT_BUTTON_UNSUPPORTED,  // -			    	0x98 	Unassigned
			INPUT_BUTTON_UNSUPPORTED,  // -			    	0x99 	Unassigned
			INPUT_BUTTON_UNSUPPORTED,  // -			    	0x9A 	Unassigned
			INPUT_BUTTON_UNSUPPORTED,  // -			    	0x9B 	Unassigned
			INPUT_BUTTON_UNSUPPORTED,  // -			    	0x9C 	Unassigned
			INPUT_BUTTON_UNSUPPORTED,  // -			    	0x9D 	Unassigned
			INPUT_BUTTON_UNSUPPORTED,  // -			    	0x9E 	Unassigned
			INPUT_BUTTON_UNSUPPORTED,  // -			    	0x9F 	Unassigned
			INPUT_BUTTON_UNSUPPORTED,  // VK_LSHIFT 		0xA0 	Left SHIFT key
			INPUT_BUTTON_UNSUPPORTED,  // VK_RSHIFT 		0xA1 	Right SHIFT key
			INPUT_BUTTON_UNSUPPORTED,  // VK_LCONTROL 		0xA2 	Left CONTROL key
			INPUT_BUTTON_UNSUPPORTED,  // VK_RCONTROL 		0xA3 	Right CONTROL key
			INPUT_BUTTON_UNSUPPORTED,  // VK_LMENU 			0xA4 	Left ALT key
			INPUT_BUTTON_UNSUPPORTED,  // VK_RMENU 			0xA5 	Right ALT key
			INPUT_BUTTON_UNSUPPORTED,  // VK_BROWSER_BACK 	0xA6 	Browser Back key
			INPUT_BUTTON_UNSUPPORTED,  // VK_BROWSER_FORWARD 0xA7 	Browser Forward key
			INPUT_BUTTON_UNSUPPORTED,  // VK_BROWSER_REFRESH 0xA8 	Browser Refresh key
			INPUT_BUTTON_UNSUPPORTED,  // VK_BROWSER_STOP 	0xA9 	Browser Stop key
			INPUT_BUTTON_UNSUPPORTED,  // VK_BROWSER_SEARCH 0xAA 	Browser Search key
			INPUT_BUTTON_UNSUPPORTED,  // VK_BROWSER_FAVORITES 0xAB 	Browser Favorites key
			INPUT_BUTTON_UNSUPPORTED,  // VK_BROWSER_HOME 	0xAC 	Browser Start and Home key
			INPUT_BUTTON_UNSUPPORTED,  // VK_VOLUME_MUTE 	0xAD 	Volume Mute key
			INPUT_BUTTON_UNSUPPORTED,  // VK_VOLUME_DOWN 	0xAE 	Volume Down key
			INPUT_BUTTON_UNSUPPORTED,  // VK_VOLUME_UP 		0xAF 	Volume Up key
			INPUT_BUTTON_UNSUPPORTED,  // VK_MEDIA_NEXT_TRACK 0xB0 	Next Track key
			INPUT_BUTTON_UNSUPPORTED,  // VK_MEDIA_PREV_TRACK 0xB1 	Previous Track key
			INPUT_BUTTON_UNSUPPORTED,  // VK_MEDIA_STOP 	0xB2 	Stop Media key
			INPUT_BUTTON_UNSUPPORTED,  // VK_MEDIA_PLAY_PAUSE 0xB3 	Play/Pause Media key
			INPUT_BUTTON_UNSUPPORTED,  // VK_LAUNCH_MAIL 	0xB4 	Start Mail key
			INPUT_BUTTON_UNSUPPORTED,  // VK_LAUNCH_MEDIA_SELECT 0xB5 	Select Media key
			INPUT_BUTTON_UNSUPPORTED,  // VK_LAUNCH_APP1 	0xB6 	Start Application 1 key
			INPUT_BUTTON_UNSUPPORTED,  // VK_LAUNCH_APP2 	0xB7 	Start Application 2 key
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xB8 	Reserved
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xB9 	Reserved
			INPUT_BUTTON_UNSUPPORTED,  // VK_OEM_1 	    	0xBA 	;
			INPUT_BUTTON_UNSUPPORTED,  // VK_OEM_PLUS 	    0xBB 	=
			INPUT_BUTTON_UNSUPPORTED,  // VK_OEM_COMMA     	0xBC 	,
			INPUT_BUTTON_UNSUPPORTED,  // VK_OEM_MINUS     	0xBD 	-
			INPUT_BUTTON_UNSUPPORTED,  // VK_OEM_PERIOD    	0xBE 	.
			INPUT_BUTTON_UNSUPPORTED,  // VK_OEM_2 	    	0xBF 	/
			INPUT_BUTTON_UNSUPPORTED,  // VK_OEM_3 	    	0xC0 	`
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xC1 	Reserved
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xC2 	Reserved
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xC3 	Reserved
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xC4 	Reserved
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xC5 	Reserved
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xC6 	Reserved
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xC7 	Reserved
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xC8 	Reserved
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xC9 	Reserved
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xCA 	Reserved
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xCB 	Reserved
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xCC 	Reserved
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xCD 	Reserved
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xCE 	Reserved
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xCF 	Reserved
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xD0 	Reserved
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xD1 	Reserved
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xD2 	Reserved
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xD3 	Reserved
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xD4 	Reserved
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xD5 	Reserved
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xD6 	Reserved
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xD7 	Reserved
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xD8	Unassigned
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xD9	Unassigned
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xDA	Unassigned
			INPUT_BUTTON_UNSUPPORTED,  // VK_OEM_4 	    	0xDB 	[
			INPUT_BUTTON_UNSUPPORTED,  // VK_OEM_5 	    	0xDC 	backslash
			INPUT_BUTTON_UNSUPPORTED,  // VK_OEM_6 	    	0xDD 	]
			INPUT_BUTTON_UNSUPPORTED,  // VK_OEM_7 	    	0xDE 	'
			INPUT_BUTTON_UNSUPPORTED,  // VK_OEM_8 	    	0xDF 	(unknown)
			INPUT_BUTTON_UNSUPPORTED,  // VK_ICO_F17 	    0xE0 	F17 on Olivetti extended keyboard (internal use only)
			INPUT_BUTTON_UNSUPPORTED,  // VK_ICO_F18 	    0xE1 	F18 on Olivetti extended keyboard (internal use only)
			INPUT_BUTTON_UNSUPPORTED,  // VK_OEM_102 	    0xE2 	< or | on IBM-compatible 102 enhanced non-U.S. keyboard
			INPUT_BUTTON_UNSUPPORTED,  // VK_ICO_HELP 	    0xE3 	Help on Olivetti extended keyboard (internal use only)
			INPUT_BUTTON_UNSUPPORTED,  // VK_ICO_00 	    0xE4 	00 on Olivetti extended keyboard (internal use only)
			INPUT_BUTTON_UNSUPPORTED,  // VK_ICO_CLEAR     	0xE6 	Clear on Olivette extended keyboard (internal use only)
			INPUT_BUTTON_UNSUPPORTED,  // - 				0xE8 	Unassigned
			INPUT_BUTTON_UNSUPPORTED,  // VK_OEM_RESET     	0xE9 	Reset (Nokia keyboards only)
			INPUT_BUTTON_UNSUPPORTED,  // VK_OEM_JUMP 	    0xEA 	Jump (Nokia keyboards only)
			INPUT_BUTTON_UNSUPPORTED,  // VK_OEM_PA1 	    0xEB 	PA1 (Nokia keyboards only)
			INPUT_BUTTON_UNSUPPORTED,  // VK_OEM_PA2 	    0xEC 	PA2 (Nokia keyboards only)
			INPUT_BUTTON_UNSUPPORTED,  // VK_OEM_PA3 	    0xED 	PA3 (Nokia keyboards only)
			INPUT_BUTTON_UNSUPPORTED,  // VK_OEM_WSCTRL    	0xEE 	WSCTRL (Nokia keyboards only)
			INPUT_BUTTON_UNSUPPORTED,  // VK_OEM_CUSEL     	0xEF 	CUSEL (Nokia keyboards only)
			INPUT_BUTTON_UNSUPPORTED,  // VK_OEM_ATTN 	    0xF0 	ATTN (Nokia keyboards only)
			INPUT_BUTTON_UNSUPPORTED,  // VK_OEM_FINNISH	0xF1 	FINNISH (Nokia keyboards only)
			INPUT_BUTTON_UNSUPPORTED,  // VK_OEM_COPY 	    0xF2 	COPY (Nokia keyboards only)
			INPUT_BUTTON_UNSUPPORTED,  // VK_OEM_AUTO 	    0xF3 	AUTO (Nokia keyboards only)
			INPUT_BUTTON_UNSUPPORTED,  // VK_OEM_ENLW 	    0xF4 	ENLW (Nokia keyboards only)
			INPUT_BUTTON_UNSUPPORTED,  // VK_OEM_BACKTAB	0xF5 	BACKTAB (Nokia keyboards only)
			INPUT_BUTTON_UNSUPPORTED,  // VK_ATTN 		    0xF6 	ATTN
			INPUT_BUTTON_UNSUPPORTED,  // VK_CRSEL 	    	0xF7 	CRSEL
			INPUT_BUTTON_UNSUPPORTED,  // VK_EXSEL 	    	0xF8 	EXSEL
			INPUT_BUTTON_UNSUPPORTED,  // VK_EREOF 	    	0xF9 	EREOF
			INPUT_BUTTON_UNSUPPORTED,  // VK_PLAY 		    0xFA 	PLAY
			INPUT_BUTTON_UNSUPPORTED,  // VK_ZOOM 		    0xFB 	ZOOM
			INPUT_BUTTON_UNSUPPORTED,  // VK_NONAME 	    0xFC 	NONAME
			INPUT_BUTTON_UNSUPPORTED,  // VK_PA1 		    0xFD 	PA1
			INPUT_BUTTON_UNSUPPORTED,  // VK_OEM_CLEAR     	0xFE 	CLEAR
		};
	}

	LRESULT CALLBACK Win32PlatformProcessMessages(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

	bool PlatformInitialize(platform_state *platformState, const char *appName, i32 width, i32 height)
	{
		platformState->data = new win32_platform_state;
		win32_platform_state *state = (win32_platform_state *)platformState->data;
		state->instance = GetModuleHandleW(nullptr);
		state->name = appName;

		// Set window class properties
		WNDCLASSA windowClass = {};
		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = Win32PlatformProcessMessages;
		windowClass.lpszClassName = appName;
		windowClass.hInstance = state->instance;
		windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		windowClass.hIcon = LoadIconA(windowClass.hInstance, MAKEINTRESOURCEA(IDI_MYAPP_ICON));
		windowClass.hCursor = LoadCursorA(0, IDC_ARROW);

		if (!RegisterClassA(&windowClass))
		{
			LOG_ERROR("RegisterClassA failed to register window class");
			return false;
		}

		state->width = width;
		state->height = height;
		// Declare the window client size
		RECT rect = {};
		rect.left = 100;
		rect.top = 100;
		rect.right = rect.left + state->width;
		rect.bottom = rect.top + state->height;

		// Adjuct the window size according to the style we
		// have for out window, while keeping the client size
		// the same.
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW | WS_VISIBLE, FALSE);
		state->width = (i16)(rect.right - rect.left);
		state->height = (i16)(rect.bottom - rect.top);

		// Create the window
		state->handle = CreateWindowA(
			appName,
			appName,
			WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			CW_USEDEFAULT, CW_USEDEFAULT, // X, Y
			state->width,
			state->height,
			0, 0,
			state->instance,
			state // By passing this here, we can access it in Win32PlatformProcessMessages using GetWindowLongPtr
		);

		if (!state->handle)
		{
			LOG_ERROR("CreateWindowA failed to create a handle.");
			return false;
		}

		RAWINPUTDEVICE rid;
		rid.usUsagePage = 0x01; // HID_USAGE_PAGE_GENERIC;
		rid.usUsage = 0x02;		// HID_USAGE_GENERIC_MOUSE;
		rid.dwFlags = 0x100;	// RIDEV_INPUTSINK;
		rid.hwndTarget = state->handle;
		if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
		{
			LOG_ERROR("RegisterRawInputDevices failed");
			return false;
		}

		ShowWindow(state->handle, SW_SHOWDEFAULT);
		SetFocus(state->handle);
		SetCapture(state->handle);
		return true;
	}

	bool PlatformTerminate(platform_state *platformState)
	{
		win32_platform_state *state = (win32_platform_state *)platformState->data;

		if (state->handle)
		{
			DestroyWindow(state->handle);
			state->handle = 0;
		}

		return true;
	}

	// Windows Specific Messages
	LRESULT CALLBACK Win32PlatformProcessMessages(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
	{
		// NOTE: The *state pointer doesn't need to be checked for null since it always gets a value
		// before we need to process other messages. On application start we get:
		// 1st message: WM_GETMINMAXINFO
		// 2nd message: WM_NCCREATE -> sets window pointer in the windows api
		win32_platform_state *state = (win32_platform_state *)GetWindowLongPtrA(handle, GWLP_USERDATA);

		LRESULT result = 0;
		switch (message)
		{
		case WM_ERASEBKGND:
			return 1;
		case WM_PAINT:
			ValidateRect(handle, 0);
			break;
		case WM_GETMINMAXINFO:
		{
			LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
			lpMMI->ptMinTrackSize.x = MIN_WINDOW_WIDTH + WIN32_WINDOW_X_BORDER;
			lpMMI->ptMinTrackSize.y = MIN_WINDOW_HEIGHT + WIN32_WINDOW_Y_BORDER;
			break;
		}
		case WM_CLOSE:
			UnregisterClassA(state->name, state->instance);
			DestroyWindow(handle);
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_SIZE: // Handle this in the generic messages process
			PostMessage(handle, WM_USER + 2, wParam, lParam);
			break;
		case WM_NCCREATE:
		{
			CREATESTRUCT *pCreate = (CREATESTRUCT *)lParam;
			if (pCreate)
			{
				win32_platform_state *state_ = (win32_platform_state *)(pCreate->lpCreateParams);
				// Set WinAPI-managed user data to store ptr to window class
				SetWindowLongPtrA(handle, GWLP_USERDATA, (LONG_PTR)(state_));
			}
			result = DefWindowProc(handle, message, wParam, lParam);
			break;
		}
		default:
			result = DefWindowProc(handle, message, wParam, lParam);
		}
		return result;
	}

	// General Use Messages
	bool PlatformProcessMessages(platform_state *platformState)
	{
		win32_platform_state *state = (win32_platform_state *)platformState->data;
		// engine_input &input = engine.input;

		MSG message;
		while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			switch (message.message)
			{
			// Order of messages when closing the window:
			// WM_CLOSE   -> Unregister window class, destroy window handle
			// WM_DESTROY -> Post Quit Message
			// WM_QUIT    -> Return false and exit message loop
			case WM_QUIT:
				return false;

			// case WM_USER + 2: // WM_SIZE
			// {
			// 	engine.onResize = true;
			// 	Win32GetWindowDim(window.handle, engine.windowWidth, engine.windowHeight);
			// 	window.width = engine.windowWidth;
			// 	window.height = engine.windowHeight;
			// 	break;
			// }
			//------------------------------------------------------------------------
			// KEYBOARD EVENTS
			//------------------------------------------------------------------------
			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			{
				InputProcessButton(Win32VkCode::toInputButton[message.wParam], true);

				if (InputIsKeyPressed(KEYBOARD_BUTTON_F4) && InputIsKeyPressed(KEYBOARD_BUTTON_ALT))
				{
					PostQuitMessage(0);
					return 0;
				}
				break;
			}
			case WM_SYSKEYUP:
			case WM_KEYUP:
			{
				InputProcessButton(Win32VkCode::toInputButton[message.wParam], false);
				break;
			}

				// //------------------------------------------------------------------------
				// // MOUSE EVENTS
				// //------------------------------------------------------------------------
				// case WM_INPUT:
				// {
				// 	UINT dwSize = sizeof(RAWINPUT);
				// 	local_var BYTE lpb[sizeof(RAWINPUT)];
				// 	if (GetRawInputData((HRAWINPUT)message.lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) == -1)
				// 	{
				// 		break;
				// 	}
			case WM_MOUSEMOVE:
			{
				POINTS p = MAKEPOINTS(message.lParam);
				p.y = 519 - p.y;
				bool isInWindow =
					p.x >= 0 && p.x < 656 &&
					p.y >= 0 && p.y < 519;
				if (true)
				{
					while (ShowCursor(TRUE) < 0)
						;
					ClipCursor(0);
				}
				else
				{
					while (ShowCursor(FALSE) >= 0)
						;
					RECT rect;
					GetClientRect(state->handle, &rect);
					MapWindowPoints(state->handle, 0, (POINT *)(&rect), 2);
					ClipCursor(&rect);
				}
				if (isInWindow)
				{
					SetCapture(state->handle);
				}
				else
				{
					ReleaseCapture();
				}
				break;
			}
			// 	// RAWINPUT *raw = (RAWINPUT *)lpb;
			// 	// if (raw->header.dwType == RIM_TYPEMOUSE &&
			// 	// 	(raw->data.mouse.lLastX != 0 || raw->data.mouse.lLastY != 0))
			// 	// {
			// 	// 	input.mouse.newPos.X += (f32)raw->data.mouse.lLastX;
			// 	// 	input.mouse.newPos.Y += (f32)raw->data.mouse.lLastY;
			// 	// }
			// 	break;
			// }
			case WM_LBUTTONDOWN:
				InputProcessButton(Win32VkCode::toInputButton[VK_LBUTTON], true);
				break;
			case WM_RBUTTONDOWN:
				InputProcessButton(Win32VkCode::toInputButton[VK_RBUTTON], true);
				break;
			case WM_MBUTTONDOWN:
				InputProcessButton(Win32VkCode::toInputButton[VK_MBUTTON], true);
				break;
			case WM_LBUTTONUP:
				InputProcessButton(Win32VkCode::toInputButton[VK_LBUTTON], false);
				break;
			case WM_RBUTTONUP:
				InputProcessButton(Win32VkCode::toInputButton[VK_RBUTTON], false);
				break;
			case WM_MBUTTONUP:
				InputProcessButton(Win32VkCode::toInputButton[VK_MBUTTON], false);
				break;
			// case WM_MOUSEWHEEL:
			// 	input.mouse.UpdateWheelDelta(GET_WHEEL_DELTA_WPARAM(message.wParam));
			// case WM_MOUSELEAVE:
			// 	POINTS p = MAKEPOINTS(message.lParam);
			// 	input.mouse.isInWindow = false;
			// 	break;
			// case WM_KILLFOCUS:
			// 	input.keyboard.Clear();
			// 	break;
			default:
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
				break;
			}
			}
		}
		return true;
	}

	read_file_result PlatformReadFile(const char *filepath)
	{
		read_file_result result = {};

		char fullFilePath[MAX_PATH] = {};
		if (GetFullPathNameA(filepath, ArrayCount(fullFilePath), fullFilePath, 0) == 0)
			return result;

		HANDLE fileHandle = CreateFileA(fullFilePath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
		if (fileHandle != INVALID_HANDLE_VALUE)
		{
			LARGE_INTEGER fileSize;
			if (GetFileSizeEx(fileHandle, &fileSize))
			{ // Truncate 64 bit value to 32 bit because VirtualAlloc only takes 32bit value
				ASSERT(fileSize.QuadPart <= 0xFFFFFFFF);
				result.size = (u32)fileSize.QuadPart;

				result.content = VirtualAlloc(0, result.size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
				if (result.content)
				{
					DWORD bytesRead;
					if (ReadFile(fileHandle, result.content, result.size, &bytesRead, 0) &&
						result.size == bytesRead)
					{
						// We read the file successfully
					}
					else
					{
						PlatformFreeFileMemory(result.content);
						result = {};
					}
				}
			}
			CloseHandle(fileHandle);
		}
		// NOTE:  We can add logging in case these steps fail.
		return result;
	}

	bool PlatformWriteFile(const char *filepath, u32 memorySize, void *memory)
	{
		bool result = false;

		char fullFilePath[MAX_PATH] = {};
		if (GetFullPathNameA(filepath, ArrayCount(fullFilePath), fullFilePath, 0) == 0)
			return result;

		HANDLE fileHandle = CreateFileA(filepath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
		if (fileHandle != INVALID_HANDLE_VALUE)
		{
			DWORD bytesWritten;
			if (WriteFile(fileHandle, memory, memorySize, &bytesWritten, 0))
			{
				// We wrote into the file successfully
				result = (bytesWritten == memorySize);
			}
			CloseHandle(fileHandle);
		}
		// NOTE:  We can add logging in case these steps fail.
		return result;
	}

	void PlatformFreeFileMemory(void *memory)
	{
		if (memory)
		{
			VirtualFree(memory, 0, MEM_RELEASE);
		}
	}

	bool PlatformGetFileWriteTime(const char *filepath, file_write_time *writeTime)
	{
		char fullFilePath[MAX_PATH] = {};
		if (GetFullPathNameA(filepath, ArrayCount(fullFilePath), fullFilePath, 0) == 0)
			return false; // Couldn't find file

		writeTime->data = malloc(sizeof(FILETIME));
		FILETIME *result = (FILETIME *)writeTime->data;
		WIN32_FIND_DATA data = {};
		HANDLE handle = FindFirstFileA(fullFilePath, (LPWIN32_FIND_DATAA)&data);
		if (handle != INVALID_HANDLE_VALUE)
		{
			*result = data.ftLastWriteTime;
			FindClose(handle);
		}
		return true;
	}

	bool PlatformWasFileUpdated(const char *filepath, file_write_time *writeTime)
	{
		if (!writeTime->data)
		{
			return false;
		}

		file_write_time newWriteTime;
		PlatformGetFileWriteTime(filepath, &newWriteTime);

		FILETIME *win32newWriteTime = (FILETIME *)writeTime->data;
		FILETIME *win32oldWriteTime = (FILETIME *)writeTime->data;
		if (CompareFileTime(win32newWriteTime, win32oldWriteTime) == 1)
		{
			*win32oldWriteTime = *win32newWriteTime;
			return true;
		}
		return false;
	}

	void Win32GetWindowDim(HWND handle, u32 &width, u32 height)
	{
		RECT rect = {};
		GetWindowRect(handle, &rect);
		width = (u32)(rect.right - rect.left);
		height = (u32)(rect.bottom - rect.top);
	}

	// Copied from: https://github.com/travisvroman/kohi/commit/ca0600eaefd11ed674c5a4642fb13ce17a96656f#diff-7f4ba46fd3ad1ae4558ae188a098f3a9d7009e1dbfc890e6562602f0f790e1e5
	void PlatformPrint(const char *message, u8 colour)
	{
		HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		// ERROR, WARN, INFO, DEBUG
		static u8 levels[] = {64, 6, 2, 1};
		SetConsoleTextAttribute(console_handle, levels[colour]);
		OutputDebugStringA(message);
		size_t length = strlen(message);
		LPDWORD number_written = 0;
		WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)length, number_written, 0);
	}

	// Copied from: https://github.com/travisvroman/kohi/commit/ca0600eaefd11ed674c5a4642fb13ce17a96656f#diff-7f4ba46fd3ad1ae4558ae188a098f3a9d7009e1dbfc890e6562602f0f790e1e5
	void PlatformPrintError(const char *message, u8 colour)
	{
		HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
		// ERROR, WARN, INFO, DEBUG
		static u8 levels[] = {64, 6, 2, 1};
		SetConsoleTextAttribute(console_handle, levels[colour]);
		OutputDebugStringA(message);
		size_t length = strlen(message);
		LPDWORD number_written = 0;
		WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), message, (DWORD)length, number_written, 0);
	}

	bool PlatformLoadDynamicLibrary(const char *filepath, dynamic_library *libOut)
	{
		strcpy(libOut->name, filepath);

		libOut->data = new win32_dll;
		win32_dll *dll = (win32_dll *)libOut->data;

		const char *dllPath = filepath;
		char filepathNoExtension[MAX_PATH];

		u32 filepathLength = (u32)strlen(filepath);
		u32 pos = 0;
		for (pos = 0; pos < filepathLength; pos++)
		{
			if (filepath[pos] != '.')
			{
				filepathNoExtension[pos] = filepath[pos];
			}
			else
			{
				filepathNoExtension[pos] = '\0';
			}
		}

		char dllCopyPath[MAX_PATH];
		sprintf(dllCopyPath, "%s_copy.dll\0", filepathNoExtension);

		// NOTE:  We need to add a sleep in order to wait for the dll compilation.
		Sleep(800);
		char dllFullPath[MAX_PATH] = {};
		if (GetFullPathNameA(dllPath, ArrayCount(dllFullPath), dllFullPath, 0) == 0)
			return false;

		char dllCopyFullPath[MAX_PATH] = {};
		if (GetFullPathNameA(dllCopyPath, ArrayCount(dllCopyFullPath), dllCopyFullPath, 0) == 0)
			return false;

		if (!PlatformGetFileWriteTime(dllFullPath, &libOut->writeTime))
			return false;

		if (CopyFileA(dllFullPath, dllCopyFullPath, FALSE))
		{
			dll->dll = LoadLibraryA(dllCopyFullPath);
			if (dll->dll)
			{
				LOG_DEBUG("Loaded '%s'", filepath);
				return true;
			}
		}

		return false;
	}

	void *PlatformGetDynamicLibraryFunction(dynamic_library *lib, const char *function)
	{
		void *result = 0;
		if (lib->data)
		{
			win32_dll *dll = (win32_dll *)lib->data;
			if (dll->dll)
			{
				result = GetProcAddress(dll->dll, function);
				if (!result)
					LOG_ERROR("Function '%s' not found in dll", function);
			}
		}
		return result;
	}

	bool PlatformUnloadDynamicLibrary(dynamic_library *lib)
	{
		win32_dll *dll = (win32_dll *)lib->data;
		if (dll->dll)
		{
			FreeLibrary(dll->dll);
			if (lib->writeTime.data)
				free(lib->writeTime.data);
			delete dll;
			return true;
		}
		return false;
	}

	bool PlatformUpdatedDynamicLibrary(dynamic_library *lib)
	{
		bool result = false;
		file_write_time newWriteTime;
		PlatformGetFileWriteTime(lib->name, &newWriteTime);

		FILETIME *oldFILETIME = (FILETIME *)lib->writeTime.data;
		FILETIME *newFILETIME = (FILETIME *)newWriteTime.data;
		if (CompareFileTime(newFILETIME, oldFILETIME) == 1)
		{
			oldFILETIME->dwLowDateTime = newFILETIME->dwLowDateTime;
			oldFILETIME->dwHighDateTime = newFILETIME->dwHighDateTime;
			result = true;
		}

		if (newWriteTime.data)
			free(newWriteTime.data);
		return result;
	}

	bool PlatformReloadDynamicLibrary(dynamic_library *lib)
	{
		if (PlatformUpdatedDynamicLibrary(lib))
		{
			PlatformUnloadDynamicLibrary(lib);
			PlatformLoadDynamicLibrary(lib->name, lib);

			LOG_DEBUG("Reloaded '%s", lib->name);
			return true;
		}
		return false;
	}

#if 0
bool Win32InitializeMemory(engine_platform *engine)
{
	engine_memory &memory = engine->memory;

	LPVOID baseAddress = (LPVOID)TERABYTES(2);
	memory.permanentMemorySize = MEGABYTES(64);
	memory.transientMemorySize = GIGABYTES(2);
	u64 totalSize = memory.permanentMemorySize + memory.transientMemorySize;
	memory.permanentMemory = VirtualAlloc(baseAddress, (SIZE_T)totalSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	memory.transientMemory = (u8 *)memory.permanentMemory + memory.permanentMemorySize;
	memory.isInitialized = false;

	/*memory.platformAPI_.FreeFileMemory = Win32FreeFileMemory;
	memory.platformAPI_.ReadFile = Win32ReadFile;
	memory.platformAPI_.WriteFile = Win32WriteFile;
	memory.platformAPI_.GetFileWriteTime = Win32GetWriteTime;
	memory.platformAPI_.WasFileUpdated = Win32FileUpdated;*/

	engine->platformAPI.FreeFileMemory = Win32FreeFileMemory;
	engine->platformAPI.ReadFile = Win32ReadFile;
	engine->platformAPI.WriteFile = Win32WriteFile;
	engine->platformAPI.GetFileWriteTime = Win32GetWriteTime;
	engine->platformAPI.WasFileUpdated = Win32FileUpdated;

	// platformAPI = memory.platformAPI_;

	bool result = memory.permanentMemory && memory.transientMemory;
	DebugPrintFunctionResult(result);
	return result;
}
#endif
}
#endif