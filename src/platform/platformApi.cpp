#include "platformApi.h"

struct Test {
  int x;
  Test(): x{0} {};
  void setX(int _x) {x = _x;};
  
};

static void SetX(Test* test, int _x) {
  test->x = _x;
};


void StartEngine()
{
  auto wnd  = Platform::PCreateWindow("Hello World", 150, 150, 600, 300);
  Platform::PShowWindow(wnd);
  
  Platform::LogDebugInfo(L"In while loop");

   while(1) {
    Platform::PPollWindowEvents(wnd);
    auto mouse = Platform::PGetMouse();

    if(mouse->left.isDown)
    {
      Platform::PSetFullscreen(wnd, true);
    } else if(mouse->right.isDown) {
      Platform::PSetFullscreen(wnd, false);
    }
   }
  
  Platform::PDestroyWindow(wnd);
 
}

void ShutdownEngin()
{
}
