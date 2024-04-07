#pragma once

#include "Valhalla/Core/Base.h"
#include "Log.h"

#ifdef VH_PLATFORM_WINDOWS
#define VH_DEBUG_BREAK __debugbreak()
#elif defined(VH_COMPILER_CLANG)
#define VH_DEBUG_BREAK __builtin_debugtrap()
#else
#define VH_DEBUG_BREAK
#endif

#ifdef VH_DEBUG
#define VH_ENABLE_ASSERTS
#endif

#define VH_ENABLE_VERIFY

#pragma once

#include "Log.h"


#ifdef VH_DEBUG
#define VH_ENABLE_ASSERTS
#endif

#ifdef VH_ENABLE_ASSERTS
#define VH_ASSERT_NO_MESSAGE(condition) { if(!(condition)) { VH_ERROR("Assertion Failed"); __debugbreak(); } }
#define VH_ASSERT_MESSAGE(condition, ...) { if(!(condition)) { VH_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

#define VH_ASSERT_RESOLVE(arg1, arg2, macro, ...) macro
#define VH_GET_ASSERT_MACRO(...) VH_EXPAND_VARGS(VH_ASSERT_RESOLVE(__VA_ARGS__, VH_ASSERT_MESSAGE, VH_ASSERT_NO_MESSAGE))

#define VH_ASSERT(...) VH_EXPAND_VARGS( VH_GET_ASSERT_MACRO(__VA_ARGS__)(__VA_ARGS__) )
#define VH_CORE_ASSERT(...) VH_EXPAND_VARGS( VH_GET_ASSERT_MACRO(__VA_ARGS__)(__VA_ARGS__) )
#else
#define VH_ASSERT(...)
#define VH_CORE_ASSERT(...)
#endif

#ifdef VH_ENABLE_VERIFY
#define VH_VERIFY_NO_MESSAGE(condition) { if(!(condition)) { VH_ERROR("Verify Failed"); __debugbreak(); } }
#define VH_VERIFY_MESSAGE(condition, ...) { if(!(condition)) { VH_ERROR("Verify Failed: {0}", __VA_ARGS__); __debugbreak(); } }

#define VH_VERIFY_RESOLVE(arg1, arg2, macro, ...) macro
#define VH_GET_VERIFY_MACRO(...) VH_EXPAND_VARGS(VH_VERIFY_RESOLVE(__VA_ARGS__, VH_VERIFY_MESSAGE, VH_VERIFY_NO_MESSAGE))

#define VH_VERIFY(...) VH_EXPAND_VARGS( VH_GET_VERIFY_MACRO(__VA_ARGS__)(__VA_ARGS__) )
#define VH_CORE_VERIFY(...) VH_EXPAND_VARGS( VH_GET_VERIFY_MACRO(__VA_ARGS__)(__VA_ARGS__) )
#else
#define VH_VERIFY(...)
#define VH_CORE_VERIFY(...)
#endif
