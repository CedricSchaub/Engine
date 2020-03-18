#include "platformApi.h"

namespace Platform {

  int64_t queryTime() {
    LARGE_INTEGER timestamp;
    QueryPerformanceCounter(&timestamp);

    return (int64_t)timestamp.QuadPart;
  };

  int64_t queryFrequency() {
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return (int64_t)frequency.QuadPart;
  };
  
}
