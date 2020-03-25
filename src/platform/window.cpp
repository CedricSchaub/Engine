#include "window.h"

bool Window::createWindow(const char* title, int32_t x, int32_t y, int32_t w, int32_t h) {
  bool result = Platform::PCreateWindow(mPlatformWindow, title, x, y, w, h);
  return result;
};

bool Window::showWindow() {
  return PShowWindow(mPlatformWindow);
};

bool Window::pollEvents() {
  return Platform::PPollWindowEvents(mPlatformWindow);
};

bool Window::setFullscreen(bool fullscreen) {
  return Platform::PSetFullscreen(mPlatformWindow, fullscreen);
};

Window::~Window() {
    Platform::PDestroyWindow(mPlatformWindow);
    if (mPlatformWindow.mWindowData)
        delete mPlatformWindow.mWindowData;
}