#pragma once

#include <memory>
#include "Ref.h"

namespace Valhalla
{
	void InitializeCore();
	void ShutdownCore();
}

#ifndef VH_PLATFORM_WINDOWS
	#error Valhalla only support windows at the moment
#endif

#define VH_EXPAND_VARGS(x) x


#define BIT(x) (1u << x)


#define VH_BIND_EVENT_FN(fn) std::bind(&##fn, this std::placeholders::_1)

#include "Assert.h"
