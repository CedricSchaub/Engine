#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

#include <cstdint>
#include <assert.h>
#include <stdio.h>

// NOTE: Platform specific data. Declare before platform types because they need to define the struct
namespace Platform {
    struct NativeWindowData;
}



#if defined(ENGINE_WINDOWS)
// NOTE: Include WindowsPlatform.h, WindowsTypes.h
#include "windowsTypes.h"
#include "windowsKeyTable.h"

#elif defined(ENGINE_LINUX)

// NOTE: Include LinuxPlatform.h, LinuxTypes.h

#endif

namespace Platform {

    struct PlatformWindow {
        int32_t width;
        int32_t height;
        int32_t xPos;
        int32_t yPos;
        bool isFullscreen;
        bool isActivated;
        const char* windowTitle;
        NativeWindowData* mWindowData;
    };

  struct InputState {
    uint64_t lastKeyPress;
    uint8_t count;
    bool isDown;
  };
  
  struct MouseInput {
    int32_t xPos;
    int32_t yPos;
    InputState left;
    InputState right;
  };

  struct KeyboardInput {
    InputState keys[KEY_COUNT];
  };
  
}

#endif
