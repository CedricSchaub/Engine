#include "platformApi.h"
#include "window.h"

void StartEngine()
{
    Window wnd;
    wnd.createWindow("Hello", 150, 150, 600, 300);
    wnd.showWindow();

    // double timer = 0.0;
    // NOTE: COUNTS PER SECOND
    
    int64_t clockFrequency = Platform::queryFrequency();
    int64_t start, end;
    double timer = 0.0;
    

    auto& mouse = Platform::PGetMouse();
    auto& keyboard = Platform::PGetKeyboard();
    bool fire = true;

    while(1) {
    
        start = Platform::queryTime();
        wnd.pollEvents();
    
		if (wnd.isActivated()) {
			Platform::LogDebugInfo("Activated\n");
		}
		else {
			Platform::LogDebugInfo("Deactivated\n");
		}
    
        end = (Platform::queryTime() - start);
        timer += end * (1.0 / clockFrequency);
        //Platform::LogDebugInfo("Timer", timer);
    }
}

void ShutdownEngin()
{
}
