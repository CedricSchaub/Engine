#pragma once
#include "platformTypes.h"

void StartEngine();
void ShutdownEngin();


namespace Platform {
  // Window stuff
 extern Window* PCreateWindow(const char*, int32_t, int32_t, int32_t, int32_t);
 extern bool PPollWindowEvents(Window*);
 extern bool PSetFullscreen(Window*, bool);
 extern bool PShowWindow(Window*);
 extern bool PDestroyWindow(Window*);

  // Input
 extern MouseInput& PGetMouse();
 extern KeyboardInput& PGetKeyboard();
  
 // Timing
 extern int64_t queryTime();
 extern int64_t queryFrequency();

 // Debugging stuff
  extern void LogDebugInfo(const ENGINE_STRING*);
};


