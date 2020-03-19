#include "window.h"

Window::~Window() {
  if(mWindow)
  {
    if(mWindow->nativeWindowData)
      delete mWindow->nativeWindowData;

    delete mWindow;
  }
}

bool Window::createWindow(const char* title, int32_t x, int32_t y, int32_t w, int32_t h) {
  mWindow = Platform::PCreateWindow(title, x, y, w, h);
  return (mWindow != nullptr);
};

bool Window::showWindow() {
  return PShowWindow(mWindow);
};

bool Window::pollEvents() {
  return Platform::PPollWindowEvents(mWindow);
};

bool Window::setFullscreen(bool fullscreen) {
  return Platform::PSetFullscreen(mWindow, fullscreen);
};
