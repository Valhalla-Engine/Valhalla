export module Valhalla.Core.Assert;

import Valhalla.Core.Log;
import <string_view>;
import <format>;

export namespace Valhalla
{
    [[noreturn]] inline void DebugBreak()
    {
#if defined(VH_PLATFORM_WINDOWS)
        __debugbreak();
#elif defined(VH_COMPILER_CLANG)
        __builtin_debugtrap();
#else
        // fallback
#endif
    }

    template<typename... Args>
    inline void Assert(bool condition, std::string_view msg = {}, Args&&... args)
    {
#ifdef VH_ENABLE_ASSERTS
        if (!condition)
        {
            if constexpr (sizeof...(args) > 0)
                Log::CoreError("Assertion Failed: {0}", std::format(msg, std::forward<Args>(args)...));
            else if (!msg.empty())
                Log::CoreError("Assertion Failed: {0}", msg);
            else
                Log::CoreError("Assertion Failed");

            DebugBreak();
        }
#endif
    }

    template<typename... Args>
    inline void Verify(bool condition, std::string_view msg = {}, Args&&... args)
    {
#ifdef VH_ENABLE_VERIFY
        if (!condition)
        {
            if constexpr (sizeof...(args) > 0)
                Log::CoreError("Verify Failed: {0}", std::format(msg, std::forward<Args>(args)...));
            else if (!msg.empty())
                Log::CoreError("Verify Failed: {0}", msg);
            else
                Log::CoreError("Verify Failed");

            DebugBreak();
        }
#endif
    }
}