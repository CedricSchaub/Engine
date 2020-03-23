#ifndef UNICODE
#define UNICODE
#endif
 
#include <assert.h>
#include <cstring>
#include "platformApi.h"

using Platform::windows_key_table;

static HINSTANCE gHinstance = {};

/*
	Internal stuff
*/
/*
  NOTE: Is forward declared in PlatformTypes.h and has to be implementet per Platform.
*/
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

  auto& mouse = Platform::PGetMouse();
  auto& keyboard = Platform::PGetKeyboard();
  
  LRESULT result = 0;
  switch(uMsg) {

    /*
      FIXME: Button UP / DOWN entprellen
     */

  case WM_CREATE: {
    SetProcessDPIAware();
  }  break;
  case WM_NCCALCSIZE: {
    int x = 0;
    x = 5;
    RECT re;
    auto r = GetClientRect(hwnd, &re);
    int xxx = 0;
  } break;
    
  case WM_MOUSEMOVE: {
    int32_t x = GET_X_LPARAM(lParam);
    int32_t y = GET_X_LPARAM(lParam);

    mouse.xPos = x;
    mouse.yPos = y;
    return 0;
  } break;
    
  case WM_LBUTTONUP: {
    mouse.left.isDown = false;
    return 0;
  } break;

  case WM_LBUTTONDOWN: {
    mouse.left.isDown = true;
    return 0;
  } break;

  case WM_RBUTTONUP: {
    mouse.right.isDown = false;
    return 0;
  } break;

  case WM_RBUTTONDOWN: {
    mouse.right.isDown = true;
    return 0;
  } break;

  case WM_KEYDOWN: {
    uint8_t previousKeyState = (lParam & (1 << 30));
    Platform::ENGINE_KEYS key = windows_key_table[wParam];
    
    if( !previousKeyState ) {
      keyboard.keys[key].isDown = true;
    }
    return 0;
    
  } break;

  case WM_KEYUP: {
    uint8_t key = windows_key_table[wParam];
    keyboard.keys[key].isDown = false;
    return 0;
  } break;
    
  }

  return  DefWindowProc(hwnd, uMsg, wParam, lParam);

};

static bool _registerWindowClass(HINSTANCE hInstance) {
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"ENGINE1";
	return RegisterClass(&wc);
};

/*
	Platform stuff
*/
namespace Platform {

  Window* PCreateWindow(const char* title, int32_t x, int32_t y, int32_t width, int32_t height) {

    Window * wnd = new Window();
    wnd->nativeWindowData = new NativeWindowData();
    wnd->width = width;
    wnd->height = height;
    wnd->xPos = x;
    wnd->yPos = y;
    wnd->windowTitle = title;
    wnd->isFullscreen = false;

    constexpr uint16_t bufferSize = 1024;
    assert(std::strlen(title) <= bufferSize);
    wchar_t buffer[bufferSize];
    
    /*
      FIXME: According to MSDN using MultiByteToWideChar can cause security issues (buffer overrun)
      Better option to convert const char* to const wchar_t* ?
    */
    int result = MultiByteToWideChar(CP_ACP, 0, title, -1, buffer, bufferSize);
    
    
    HWND hwnd = CreateWindowEx(0, L"ENGINE1", buffer, WS_OVERLAPPEDWINDOW, x, y, width, height, NULL, NULL, gHinstance, NULL);
    assert(hwnd != NULL);
	
    wnd->nativeWindowData->windowHandle = hwnd;
    wnd->nativeWindowData->hinstance = gHinstance;
    return wnd;
  }

  /*
    NOTE: As long as there are window events, process them
  */
  bool PPollWindowEvents(Window* wnd) {

    MSG msg;
    HWND hwnd = wnd->nativeWindowData->windowHandle;

    /*
      NOTE:
      PeekMessage() only returns messages that were placed in the thread's
      message queue with PostMessage(). However, if a thread is waiting in a
      call to SendMessage(), that message will be handled by a direct call to
      the window procedure before PeekMessage() returns.
     */
    while(PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
	DispatchMessage(&msg);
    }
    
    return false;
  }

  bool PSetFullscreen(Window* window, bool fullscreen) {
    
    WINDOWPLACEMENT* wndPlacement = &window->nativeWindowData->windowPlacement;
    LONG_PTR* dwStyles = &window->nativeWindowData->dwStyles;
    HWND windowHandle = window->nativeWindowData->windowHandle;
    
    // NOTE: If window is already in fullscreen mode, go window mode else go fullscreen
    if(!window->isFullscreen && fullscreen) {
      

      MONITORINFO* mi = &window->nativeWindowData->monitor;
      auto sz = sizeof(MONITORINFO);
      
      BOOL result = GetWindowPlacement(windowHandle, wndPlacement);
      
      result = GetMonitorInfo(MonitorFromWindow(windowHandle, MONITOR_DEFAULTTOPRIMARY), mi);

    auto msg = GetLastError();

	   if(!result) return false;

	*dwStyles = GetWindowLongPtr(windowHandle, GWL_STYLE);
	
	SetWindowLongPtr(windowHandle, GWL_STYLE, *dwStyles & ~WS_OVERLAPPEDWINDOW);

	SetWindowPos(windowHandle, HWND_TOP,
		     mi->rcMonitor.left, mi->rcMonitor.top,
		     mi->rcMonitor.right - mi->rcMonitor.left,
		     mi->rcMonitor.bottom - mi->rcMonitor.top,
		     SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	
	window->isFullscreen = true;	                               
      
    }  else if(window->isFullscreen && !fullscreen) {

	SetWindowLongPtr(windowHandle, GWL_STYLE, *dwStyles);
	/*
	
	  NOTE: From MSDN ->
	  Before calling SetWindowPlacement, set the length member of the WINDOWPLACEMENT structure to sizeof(WINDOWPLACEMENT). 
          SetWindowPlacement fails if the length member is not set correctly.
	 
	*/
	wndPlacement->length = sizeof(WINDOWPLACEMENT);
	SetWindowPlacement(windowHandle, wndPlacement);
	SetWindowPos(windowHandle, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
		     SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	window->isFullscreen = false;
    }

    return true;
  }; 

  bool PShowWindow(Window* window) {
    // 5 = Flag for activating window and displayiong it with its current size and position
    return ShowWindow(window->nativeWindowData->windowHandle, 5);
  }

  bool PDestroyWindow(Window* window) {

    assert(window);
    assert(window->nativeWindowData);
  
    bool result = CloseWindow(window->nativeWindowData->windowHandle);

    delete window->nativeWindowData;
    delete window;

    return result;
  }

  void LogDebugInfo(const ENGINE_STRING* msg) {
    
    OutputDebugString(msg);
  }
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int nCmdShow) {
	assert(_registerWindowClass(hInstance));
	gHinstance = hInstance;
	StartEngine();

	return 0;
}
