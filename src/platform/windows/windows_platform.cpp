#include <cstring>
#include "internal.h"
#include "platformApi.h"


using Platform::windows_key_table;

static HINSTANCE gHinstance = {};

static const wchar_t ClassName[] = L"ENGINE";
static const wchar_t OpenGLClassName[] = L"OENGINE";

static int keyCounter = 0;

struct OpenGLVersion {
    int major;
    int minor;
};

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

        /*
            lParam 
            bit 0 - 15 = repeat counter
            bit 30 = prev. key state was it down or up ?
        */
        bool previousKeyState = (lParam & (1 << 30));

        Platform::ENGINE_KEYS key = windows_key_table[wParam];
    
        if( !previousKeyState ) {
          keyboard.keys[key].isDown = true;
          Platform::LogDebugInfo("Key was pressed\n");
        }

      } break;

      case WM_KEYUP: {

        uint8_t key = windows_key_table[wParam];
        keyboard.keys[key].isDown = false;

      } break;

      case WM_DESTROY: {
           PostQuitMessage(0);
           window->closeRequested = true;
      } break;

      default: {
          result = DefWindowProc(hwnd, uMsg, wParam, lParam);
      } break;
    
  }

  return result;
};

static bool _registerWindowClass(HINSTANCE hInstance, bool opengl) {
	WNDCLASS wc = {};
	wc.lpfnWndProc = (opengl) ? DefWindowProc : WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = (opengl) ? OpenGLClassName : ClassName;
    wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	return RegisterClass(&wc);
};

static bool _unregisterWindowClass(HINSTANCE hInstance, bool opengl) {
    
    bool result = false;

    if (opengl) {
        result = UnregisterClass(OpenGLClassName, hInstance);
    }
    else {

       result = UnregisterClass(ClassName, hInstance);
    }

    if (!result) {
        auto error = GetLastError();
        int x = 0;
    }

    return result;
};

static void* _getOpenGLFunctionAddress(const char* functionName) {
    /*
        khronos.org
        While the MSDN documentation says that wglGetProcAddress returns NULL on failure, some implementations will return other values. 1, 2, and 3 are used, as well as -1.
    */
    void* address = wglGetProcAddress(functionName);
    if (address == NULL || address == (void*)0x1 || address == (void*)0x2 || address == (void*)0x3 || address == (void*)-1) {
        assert(false);
    }

    return address;
}

static OpenGLVersion _getOpenGLVersion() {
    int major, minor;
    major = minor = 0;

    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    return { major, minor };
};

static void _getOpenGLExtensions(Platform::PlatformWindow& window) {
    assert(wglGetCurrentContext() != NULL);

    GLint numberOfExtensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &numberOfExtensions);

    PFNGLGETSTRINGIPROC glGetStringi = (PFNGLGETSTRINGIPROC)_getOpenGLFunctionAddress("glGetStringi");
    assert(glGetStringi != NULL);

    // TODO: Init. OpenGL Functions in a seperate file?

    for (GLint index = 0; index < numberOfExtensions; index++) {

        const char* extensionName = (const char*)glGetStringi(GL_EXTENSIONS, index);
        window.mOpenGLExtensions.push_back(extensionName);
    }
}

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
        PCreateOpenGLContext(window, 3, 3);

        _getOpenGLExtensions(window);
    
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
			SetWindowPos(windowHandle, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
				SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
			window.isFullscreen = false;
    }

    return true;
  }; 

  bool PShowWindow(Platform::PlatformWindow& window) {
    // 5 = Flag for activating window and displayiong it with its current size and position
    return ShowWindow(window.mWindowData->windowHandle, 1);
  }

  bool PDestroyWindow(Platform::PlatformWindow& window) {
      bool result = PDeleteOpenGLContext(window);
      assert(result);

      result = DestroyWindow(window.mWindowData->windowHandle);
    //  assert(result);

      LogDebugInfo("Window destroyed");

      return result;
  }

   bool PCreateOpenGLContext(PlatformWindow& window, int major, int minor) {
       _registerWindowClass(window.mWindowData->hinstance, true);

       HWND fakeWindow = CreateWindow(OpenGLClassName, L"FakeWindow", WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 1, 1, NULL, NULL, window.mWindowData->hinstance, NULL);
       
       HDC fakeDC = GetDC(fakeWindow);

       PIXELFORMATDESCRIPTOR pixelDescriptor{
           sizeof(PIXELFORMATDESCRIPTOR), // Size
           1, // Version
           PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
           PFD_TYPE_RGBA, // RGBA Colors
           32, // Colordepth R=8 Bit, G=8 Bit, B=8 Bit, Alpha=8 Bit => 4 * 8 Bit = 32 Bit
           0, 0, 0, 0, 0, 0,
           0,
           0,
           0,
           0, 0, 0, 0,
           24,// BitsDepthbuffer
           8, // Bits Stencilbuffer
           0, // Number of Aux Buffers ???
           PFD_MAIN_PLANE,
           0,
           0, 0, 0
       };

       int fakeID = ChoosePixelFormat(fakeDC, &pixelDescriptor);
       assert(fakeID != 1);

       bool pixelFormatSet = SetPixelFormat(fakeDC, fakeID, &pixelDescriptor);
       assert(pixelFormatSet == true);

       // Create temporary rendering context and make current

       HGLRC fakeRC = wglCreateContext(fakeDC);
       assert(fakeRC != 0);

       bool renderingContextCurrent = wglMakeCurrent(fakeDC, fakeRC);
       assert(renderingContextCurrent == true);

       PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)_getOpenGLFunctionAddress("wglChoosePixelFormatARB");
       PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)_getOpenGLFunctionAddress("wglCreateContextAttribsARB");

       // CREATE ACTUAL RENDERING CONTEXT
       // https://mariuszbartosik.com/opengl-4-x-initialization-in-windows-without-a-framework/

       const int attribList[] =
       {
           WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
           WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
           WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
           WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
           WGL_COLOR_BITS_ARB, 32,
           WGL_DEPTH_BITS_ARB, 24,
           WGL_STENCIL_BITS_ARB, 8,
           0, // End
       };

       int pixelFormat;
       UINT numFormats;

       HDC realDC = GetDC(window.mWindowData->windowHandle);
       bool result = wglChoosePixelFormatARB(realDC, attribList, NULL, 1, &pixelFormat, &numFormats);

       PIXELFORMATDESCRIPTOR PFD;
       DescribePixelFormat(realDC, pixelFormat, sizeof(PFD), &PFD);
       result = SetPixelFormat(realDC, pixelFormat, &PFD);

       int  contextAttribs[] = {
           WGL_CONTEXT_MAJOR_VERSION_ARB, major,
           WGL_CONTEXT_MINOR_VERSION_ARB, minor,
           WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
           0
       };

       HGLRC rc = wglCreateContextAttribsARB(realDC, 0, contextAttribs);

       wglMakeCurrent(NULL, NULL);
       wglDeleteContext(fakeRC);
       DestroyWindow(fakeWindow);
      
       result = wglMakeCurrent(realDC, rc);

       window.mWindowData->glRenderContext = rc;

       auto version = _getOpenGLVersion();
       
       _unregisterWindowClass(window.mWindowData->hinstance, true);

       return true;
   };
  
   bool PDeleteOpenGLContext(PlatformWindow& window) {
           /*
             MSDN: If a rendering context is the calling thread's current context, the wglDeleteContext function changes the rendering context to being not current before deleting it.
           */

           bool result = wglDeleteContext(window.mWindowData->glRenderContext);
           //assert(result);

           LogDebugInfo("OpenGL Context destroyed.");

           return result;
   }

   void LogDebugInfo(const char* msg) {
       OutputDebugStringA(msg);
   };

  void LogDebugInfo(const char* msg, double value) {
      char buffer[1024];
      sprintf(buffer, "%lf\n", value);
      OutputDebugStringA(buffer);
  };

  void PClearScreen() {
      glClearColor(0.129f, 0.586f, 0.949f, 1.0f);
  };
  
  void PSwapBuffers(PlatformWindow& window) {
      HDC dc = GetDC(window.mWindowData->windowHandle);
      SwapBuffers(dc);
  };
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int nCmdShow) {
	assert(_registerWindowClass(hInstance, false));
    gHinstance = hInstance;

	StartEngine();
    assert(_unregisterWindowClass(hInstance, false));

	return 0;
}
