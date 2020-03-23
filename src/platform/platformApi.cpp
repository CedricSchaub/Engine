#include "platformApi.h"
#include "window.h"

void StartEngine()
{
    Window wnd;
    wnd.createWindow("Hello World", 150, 150, 600, 300);
    wnd.showWindow();
  
    Platform::LogDebugInfo(L"In while loop");

    long double timer = 0.0;
    // NOTE: COUNTS PER SECOND
    auto clockFrequency = Platform::queryFrequency();
    
    while(1) {
     wnd.pollEvents();

     auto& mouse = Platform::PGetMouse();
     auto& keyboard = Platform::PGetKeyboard();

     auto start = Platform::queryTime();

     
       if(timer >= 5.0 && keyboard.keys[Platform::KEY_A].isDown) {
	 Platform::LogDebugInfo(L"A ist down");
	 wnd.setFullscreen(true);
	        timer = 0;
       }
       else if(timer >= 5.0 && keyboard.keys[Platform::KEY_S].isDown) {
	 Platform::LogDebugInfo(L"S ist down");
	 	 wnd.setFullscreen(false);
	        timer = 0;
       }

     uint64_t  end = (Platform::queryTime() - start);

    // Sekunden
     timer += ( end * (1.0/ clockFrequency));
    
    char buffer[1024];
    sprintf(buffer, "Timer: %lf\n", timer);
    OutputDebugStringA(buffer);

    }


}

void ShutdownEngin()
{
}
