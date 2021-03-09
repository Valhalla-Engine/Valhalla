#pragma once
#include <memory>
#include "Types.h"
#include "Ref.h"

namespace Valhalla
{
	void InitializeCore();
	void ShutdownCore();
}

#ifndef VH_PLATFORM_WINDOWS
	#error Valhalla only supports Windows!
#endif 

#define BIT(x) (1 << x)

#define VH_BIND_EVENT_FN(fn) std::bind(&##fn, this, std::placeholders::_1)

#define VH_EXPAND_VARGS(x) x

#include "Assert.h"

namespace Valhalla
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
}