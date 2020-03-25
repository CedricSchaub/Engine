#pragma once
#include "platformApi.h"

class Window {
    Platform::PlatformWindow mPlatformWindow;

public:
    Window() : mPlatformWindow{} {}
  ~Window();

  // NOTE: Should the window be allowed to copy ??
  Window(const Window& wnd) = delete;
  Window& operator=(const Window& rhs) = delete;
  Window(const Window&& wnd) = delete;
  Window& operator=(const Window&& rhs) = delete;
  
  bool createWindow(const char* title, int32_t, int32_t, int32_t, int32_t);
  bool showWindow();
  bool pollEvents();
  bool setFullscreen(bool);

  bool isActivated() const { return mPlatformWindow.isActivated; };
};
