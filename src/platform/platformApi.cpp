#include "platformApi.h"
#include "window.h"

using namespace Platform;

void StartEngine()
{
   // Window wnd;
   // wnd.createWindow("Open GL", 0, 0, 600, 600);
 

   // wnd.createWindow("Hello", 150, 150, 600, 300);
    //wnd.showWindow();

    Window wnd;
    wnd.createWindow("OpenGL", 0, 0, 500, 500);
    wnd.showWindow();

    // double timer = 0.0;
    // NOTE: COUNTS PER SECOND
    
 /*   int64_t clockFrequency = Platform::queryFrequency();
    int64_t start, end;
    double timer = 0.0;
    

    auto& mouse = Platform::PGetMouse();*/
    auto& keyboard = Platform::PGetKeyboard();
   

    while(!wnd.isCloseRequested()) {
    
        //start = Platform::queryTime();
        wnd.pollEvents();



        wnd.clearScreen();
        wnd.swapBuffers();
    /*
        end = (Platform::queryTime() - start);
        timer += end * (1.0 / clockFrequency);
        Platform::LogDebugInfo("Timer", timer);*/
    }
}

void ShutdownEngin()
{
}
