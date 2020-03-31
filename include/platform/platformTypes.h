#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

#include <cstdint>
#include <assert.h>
#include <stdio.h>
#include "platformKeyTable.h"
#include <vector>

// NOTE: Platform specific data. Declare before platform types because they need to define the struct
namespace Platform {
    struct NativeWindowData;

    struct PlatformWindow {
        int32_t width;
        int32_t height;
        int32_t xPos;
        int32_t yPos;
        bool isFullscreen;
        bool isActivated;
        bool closeRequested;
        const char* windowTitle;
        NativeWindowData* mWindowData;
        std::vector<const char*> mOpenGLExtensions;
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
     // TODO: Remove magic number
    InputState keys[Platform::ENGINE_KEYS::KEY_COUNT];
  };
  
}

#endif
