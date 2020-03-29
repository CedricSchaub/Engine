#ifndef WINDOWS_TYPES_H
#define WINDOWS_TYPES_H

#include <windows.h>
#include <windowsx.h>
#include <gl/GLU.h>
#include "opengl/glext.h"
#include "opengl/wglext.h"

#define WIN32_LEAN_AND_MEAN


namespace Platform {
    struct NativeWindowData {
        HWND windowHandle;
        HINSTANCE hinstance;
        WINDOWPLACEMENT windowPlacement;
        MONITORINFO monitor;
        LONG_PTR dwStyles;
        HGLRC glRenderContext;

        NativeWindowData() : windowHandle{ NULL }, hinstance{ NULL }, windowPlacement{ sizeof(WINDOWPLACEMENT) }, monitor{ sizeof(MONITORINFO) }, dwStyles{ 0 } {}
    };
}

#endif