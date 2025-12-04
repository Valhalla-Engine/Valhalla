export module Valhalla.Core;

import Valhalla.Core.Types; 

import <memory>;      
import <functional>;
import <utility>;


namespace Valhalla
{
    export void InitializeCore();
    export void ShutdownCore();
}

// Platform check
#ifndef VH_PLATFORM_WINDOWS
#error Valhalla only supports Windows!
#endif

namespace Valhalla
{
    export constexpr int BIT(int x) { return 1 << x; }

    template<typename T, typename F>
    constexpr auto BindEventFn(T* obj, F&& func) {
        return std::bind(std::forward<F>(func), obj, std::placeholders::_1);
	}
}



// Variadic expand macro (kept for compatibility)
#define VH_EXPAND_VARGS(x) x

namespace Valhalla
{
    // Scope utility (exported)
    export template<typename T>
        using Scope = std::unique_ptr<T>;

    export template<typename T, typename ... Args>
        constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
}