#ifndef WINDOWS_KEY_TABLE_H
#define WINDOWS_KEY_TABLE_H
#include "platformKeyTable.h"

namespace Platform {
//NOTE: Look at tha windows virtual key table mapping
static ENGINE_KEYS windows_key_table[]
{
      /*
        NOTE: At 0x30 do the keys start
      */
          KEY_UNDEFINED, 
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
	
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
	
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,

          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,

          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
	
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
          KEY_UNDEFINED,
	
          KEY_0,
          KEY_1,
          KEY_2,
          KEY_3,
          KEY_4,
          KEY_5,
          KEY_6,
          KEY_7,
          KEY_8,
          KEY_9,

          KEY_UNDEFINED,
          KEY_A,
          KEY_B,
          KEY_C,
          KEY_D,
          KEY_E,
          KEY_F,
          KEY_G,
          KEY_H,
          KEY_I,
	
          KEY_J,
          KEY_K,
          KEY_L,
          KEY_M,
          KEY_N,
          KEY_O,
          KEY_P,
          KEY_Q,
          KEY_R,
          KEY_S,
	
          KEY_T,
          KEY_U,
          KEY_V,
          KEY_W,
          KEY_X,
          KEY_Y,
          KEY_Z,
    };
}
#endif
