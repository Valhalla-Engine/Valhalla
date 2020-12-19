#pragma once

#include <memory>

#ifdef VH_PLATFORM_WINDOWS
	#ifdef VH_BUILD_DLL
		#define VALHALLA_API __declspec(dllexport)
	#else
		#define VALHALLA_API __declspec(dllimport)
	#endif
#else
	#ifdef VH_BUILD_DLL
		#define VALHALLA_API __declspec(dllexport)
	#else
		#define VALHALLA_API __declspec(dllimport)
	#endif 
#endif

#include "Ref.h"

#define VH_EXPAND_VARGS(x) x

#define VH_BIND_EVENT_FN(fn) std::bind(&##fn, this, std::placeholders::_1)

#include "Assert.h"

#define BIT(x) (1 << x)

namespace Valhalla
{
	void VALHALLA_API InitializeCore();

	void VALHALLA_API ShutdownCore();

	using byte = uint8_t;
}
