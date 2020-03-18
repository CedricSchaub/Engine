#pragma once

#include <Windows.h>
#include <windowsx.h>

#define WIN32_LEAN_AND_MEAN

typedef wchar_t ENGINE_STRING;

struct NativeWindowData {
	HWND windowHandle;
        HINSTANCE hinstance;
        WINDOWPLACEMENT windowPlacement;
        MONITORINFO monitor;
        LONG_PTR dwStyles;

  NativeWindowData(): windowHandle{NULL}, hinstance{NULL}, windowPlacement{sizeof(WINDOWPLACEMENT)}, monitor{sizeof(MONITORINFO)}, dwStyles{0} {}
};
