#pragma once

#include <Windows.h>
#include <windowsx.h>

#define WIN32_LEAN_AND_MEAN

namespace Platform {
    struct NativeWindowData {
        HWND windowHandle;
        HINSTANCE hinstance;
        WINDOWPLACEMENT windowPlacement;
        MONITORINFO monitor;
        LONG_PTR dwStyles;

        NativeWindowData() : windowHandle{ NULL }, hinstance{ NULL }, windowPlacement{ sizeof(WINDOWPLACEMENT) }, monitor{ sizeof(MONITORINFO) }, dwStyles{ 0 } {}
    };
}
