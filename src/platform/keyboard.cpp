#include "platformTypes.h"

static Platform::KeyboardInput mKeyboard;

namespace Platform {
  
  KeyboardInput& PGetKeyboard() {
    return mKeyboard;
  }
  
};
