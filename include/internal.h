#ifndef INTERNAL_H
#define INTERNAL_H

	#if defined(ENGINE_WINDOWS)
	// NOTE: Include WindowsPlatform.h, WindowsTypes.h
	#include "windowsTypes.h"
	#include "windowsKeyTable.h"

	#elif defined(ENGINE_LINUX)

	// NOTE: Include LinuxPlatform.h, LinuxTypes.h

	#endif

#endif