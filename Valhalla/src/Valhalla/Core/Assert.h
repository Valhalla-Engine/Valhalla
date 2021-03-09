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
