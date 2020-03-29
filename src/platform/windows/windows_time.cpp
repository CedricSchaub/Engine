#include "internal.h"
#include "platformTypes.h"

namespace Platform {

  // NOTE: Returns COUNTS.
  int64_t queryTime() {
    LARGE_INTEGER timestamp;
    QueryPerformanceCounter(&timestamp);
    return (int64_t)timestamp.QuadPart;
  };

  // NOTE: Returns COUNTS PER SECOND. Doesnt change until reboot.
  int64_t queryFrequency() {
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return (int64_t)frequency.QuadPart;
  };
  
}
