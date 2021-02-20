#pragma once

#include <memory>
#include "Type.h"
#ifndef VH_PLATFORM_WINDOWS
#error Valhalla only supports Windows!
#endif // !VH_PLATFORM_WINDOWS

// __VA_ARGS__ expansion to get past MSVC "bug"
#define VH_EXPAND_VARGS(x)  x

// Bit Mask
#define BIT(x) (1 << x)

#define VH_BIND_EVENT_FN(fn) std::bind(&##fn, this, std::placeholders::_1)

namespace Valhalla
{
	// Pointer wrapper for unique pointer
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	using byte = uint8_t;
}