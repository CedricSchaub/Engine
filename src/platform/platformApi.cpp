#include "platformApi.h"
#include "window.h"

void StartEngine()
{
  {
    Window wnd;
    wnd.createWindow("Hello World", 150, 150, 600, 300);
    wnd.showWindow();
  
    Platform::LogDebugInfo(L"In while loop");

   while(1) {
     wnd.pollEvents();
     auto mouse = Platform::PGetMouse();

    if(mouse->left.isDown)
    {
     wnd.setFullscreen(true);
    } else if(mouse->right.isDown) {
      wnd.setFullscreen(false);
    }
   }
  }
}

void ShutdownEngin()
{
}
