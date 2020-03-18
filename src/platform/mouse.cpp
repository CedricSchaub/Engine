#pragma once
#include "platformApi.h"

static Platform::MouseInput mouseInput;

namespace Platform {
  MouseInput *const PGetMouse() {
    return &mouseInput;
  }
}


