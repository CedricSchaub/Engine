#ifndef UNICODE
#define UNICODE
#endif
 
#include <cstring>
#include "platformApi.h"

using Platform::windows_key_table;

static HINSTANCE gHinstance = {};
static const wchar_t ClassName[] = L"ENGINE";

/*
	Internal stuff
*/
/*
  NOTE: Is forward declared in PlatformTypes.h and has to be implementet per Platform.
*/
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

  auto& mouse = Platform::PGetMouse();
  auto& keyboard = Platform::PGetKeyboard();

  Platform::PlatformWindow* window = (Platform::PlatformWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

  LRESULT result = 0;

  switch(uMsg) {
        /*
          FIXME: Button UP / DOWN entprellen
         */

      case WM_ACTIVATE: {
              if (wParam != WA_INACTIVE) {
                  window->isActivated = true;
                  Platform::LogDebugInfo("Window active\n");
              }
              else {
                  window->isActivated = false;
                  Platform::LogDebugInfo("Window not active\n");
              }
      } break;

      case WM_SETFOCUS: {
          Platform::LogDebugInfo("Got focus\n");
      } break;

      case WM_KILLFOCUS: {
          Platform::LogDebugInfo("Lost focus\n");
      } break;
 
      case WM_MOUSEMOVE: {

        int32_t x = GET_X_LPARAM(lParam);
        int32_t y = GET_X_LPARAM(lParam);

        mouse.xPos = x;
        mouse.yPos = y;

      } break;
    
      case WM_LBUTTONUP: {
        if(mouse.left.isDown)
            mouse.left.isDown = false;
      } break;

      case WM_LBUTTONDOWN: {
        if(!mouse.left.isDown)
            mouse.left.isDown = true;
 
      } break;

      case WM_RBUTTONUP: {
        if(mouse.right.isDown)
            mouse.right.isDown = false;

      } break;

      case WM_RBUTTONDOWN: {
        if(!mouse.right.isDown)
            mouse.right.isDown = true;
        
      } break;

      case WM_KEYDOWN: {

        uint8_t previousKeyState = (lParam & (1 << 30));
        Platform::ENGINE_KEYS key = windows_key_table[wParam];
    
        if( !previousKeyState ) {
          keyboard.keys[key].isDown = true;
        }
    
      } break;

      case WM_KEYUP: {

        uint8_t key = windows_key_table[wParam];
        keyboard.keys[key].isDown = false;

      } break;

      case WM_DESTROY: {
          PostQuitMessage(0);
      } break;

      default: {
          result = DefWindowProc(hwnd, uMsg, wParam, lParam);
      } break;
    
  }

  return result;
};

static bool _registerWindowClass(HINSTANCE hInstance) {
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = ClassName;
	return RegisterClass(&wc);
};

/*
	Platform stuff
*/
namespace Platform {

    bool PCreateWindow(Platform::PlatformWindow& window, const char* title, int32_t x, int32_t y, int32_t width, int32_t height) {

        window.mWindowData = new Platform::NativeWindowData();

		constexpr uint16_t bufferSize = 1024;
        assert(std::strlen(title) <= bufferSize);
        wchar_t buffer[bufferSize];

        /*
          FIXME: According to MSDN using MultiByteToWideChar can cause security issues (buffer overrun)
          Better option to convert const char* to const wchar_t* ?
        */
        int result = MultiByteToWideChar(CP_ACP, 0, title, -1, buffer, bufferSize);
 
        HWND hwnd = CreateWindowEx(0, ClassName, buffer, WS_OVERLAPPEDWINDOW, x, y, width, height, NULL, NULL, gHinstance, NULL);
	
        assert(hwnd != NULL);

        window.mWindowData->windowHandle = hwnd;
        window.mWindowData->hinstance = gHinstance;

        // TODO: Should you do this ? Should I maybe save this in a linked list to allow more then one window context to be saved?
        // 
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&window);
    
        return true;
    }

  /*
    NOTE: As long as there are window events, process them
  */
  bool PPollWindowEvents(Platform::PlatformWindow& window) {

    MSG msg;
    HWND hwnd = window.mWindowData->windowHandle;
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

  bool PSetFullscreen(Platform::PlatformWindow& window, bool fullscreen) {
        
      Platform::NativeWindowData* nativeWindow = window.mWindowData;

        WINDOWPLACEMENT* wndPlacement = &nativeWindow->windowPlacement;
        LONG_PTR* dwStyles = &nativeWindow->dwStyles;
        HWND windowHandle = nativeWindow->windowHandle;
    
    // NOTE: If window is already in fullscreen mode, go window mode else go fullscreen
        if(!window.isFullscreen && fullscreen) {
			MONITORINFO* mi = &window.mWindowData->monitor;
			auto sz = sizeof(MONITORINFO);

			BOOL result = GetWindowPlacement(windowHandle, wndPlacement);

			result = GetMonitorInfo(MonitorFromWindow(windowHandle, MONITOR_DEFAULTTOPRIMARY), mi);

            assert(result != false);

			*dwStyles = GetWindowLongPtr(windowHandle, GWL_STYLE);

			SetWindowLongPtr(windowHandle, GWL_STYLE, *dwStyles & ~WS_OVERLAPPEDWINDOW);

			SetWindowPos(windowHandle, HWND_TOP,
				mi->rcMonitor.left, mi->rcMonitor.top,
				mi->rcMonitor.right - mi->rcMonitor.left,
				mi->rcMonitor.bottom - mi->rcMonitor.top,
				SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

			window.isFullscreen = true;
		}
		else if (window.isFullscreen && !fullscreen) {
			SetWindowLongPtr(windowHandle, GWL_STYLE, *dwStyles);
			/*

			  NOTE: From MSDN ->
			  Before calling SetWindowPlacement, set the length member of the WINDOWPLACEMENT structure to sizeof(WINDOWPLACEMENT).
				  SetWindowPlacement fails if the length member is not set correctly.

			*/
			wndPlacement->length = sizeof(WINDOWPLACEMENT);
			SetWindowPlacement(windowHandle, wndPlacement);
			/*SetWindowPos(windowHandle, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
				SWP_NOOWNERZORDER | SWP_FRAMECHANGED);*/
			window.isFullscreen = false;
    }

    return true;
  }; 

  bool PShowWindow(Platform::PlatformWindow& window) {
    // 5 = Flag for activating window and displayiong it with its current size and position
    return ShowWindow(window.mWindowData->windowHandle, 1);
  }

  bool PDestroyWindow(Platform::PlatformWindow& window) {

    bool result = UnregisterClass(ClassName, window.mWindowData->hinstance);
    assert(result != false);

    return result;
  }  
  
  void LogDebugInfo(const char* msg) {
      OutputDebugStringA(msg);
  }

  void LogDebugInfo(const char* msg, double value) {
      char buffer[1024];
      sprintf(buffer, "Timer: %lf\n", value);
      OutputDebugStringA(buffer);
  }
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int nCmdShow) {
	assert(_registerWindowClass(hInstance));
    gHinstance = hInstance;

	StartEngine();

	return 0;
}
