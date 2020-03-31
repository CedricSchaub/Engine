#ifndef PLATFORM_API_H
#define PLATFORM_API_H

#include "platformTypes.h"

void StartEngine();
void ShutdownEngin();

namespace Platform {
  // Window stuff
 extern bool PCreateWindow(PlatformWindow&, const char*, int32_t, int32_t, int32_t, int32_t);
 extern bool PPollWindowEvents(PlatformWindow&);
 extern bool PSetFullscreen(PlatformWindow&, bool);
 extern bool PShowWindow(PlatformWindow&);
 extern bool PDestroyWindow(PlatformWindow&);

 // Open GL stuff
 extern bool PCreateOpenGLContext(PlatformWindow&, int, int);
 extern bool PDeleteOpenGLContext(PlatformWindow&);
 extern void PClearScreen();
 extern void PSwapBuffers(PlatformWindow&);

  // Input
 extern MouseInput& PGetMouse();
 extern KeyboardInput& PGetKeyboard();
  
 // Timing
 extern int64_t queryTime();
 extern int64_t queryFrequency();

 // Debugging stuff
  extern void LogDebugInfo(const char*);
  extern void LogDebugInfo(const char*, double);
};

#endif
