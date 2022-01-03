#pragma once

#ifdef VH_DEBUG
	#define VH_ENABLE_ASSERTS
#endif

#ifdef _MSC_VER
	#define DEBUG_BREAK __debugbreak()
#endif

#define VH_ENABLE_VERIFY

#ifdef VH_ENABLE_ASSERTS
	#define VH_ASSERT_NO_MESSAGE(condition) { if(!(condition)) { VH_ERROR("Assertion Failed!"); DEBUG_BREAK; } }
	#define VH_ASSERT_MESSAGE(condition, ...) { if(!(condition)) { VH_ERROR("Assertion Failed: {0}", __VA_ARGS__); DEBUG_BREAK; } }
	
	#define VH_ASSERT_RESOLVE(arg1, arg2, macro, ...) macro
	#define VH_GET_ASSERT_MACRO(...) VH_EXPAND_VARGS(VH_ASSERT_RESOLVE(__VA_ARGS__, VH_ASSERT_MESSAGE, VH_ASSERT_NO_MESSAGE))

	#define VH_ASSERT(...) VH_EXPAND_VARGS( VH_GET_ASSERT_MACRO(__VA_ARGS__)(__VA_ARGS__))
	#define VH_CORE_ASSERT(...) VH_EXPAND_VARGS( VH_GET_ASSERT_MACRO(__VA_ARGS__)(__VA_ARGS__) )
#else
	#define VH_ASSERT(...)
	#define VH_CORE_ASSERT(...)
#endif // VH_ENABLE_ASSERTS

#ifdef VH_ENABLE_VERIFY
	#define VH_VERIFY_NO_MESSAGE(condition) {if (!(condition)) {VH_ERROR("Verify Failed!"); DEBUG_BREAK;}}
	#define VH_VERIFY_MESSAGE(condition, ...) { if (!(condition)) {VH_ERROR("Verify Failed: {0}", __VA_ARGS__); DEBUG_BREAK;}}

	#define VH_VERIFY_RESOLVE(arg1, arg2, macro, ...) macro
	#define VH_GET_VERIFY_MACRO(...) VH_EXPAND_VARGS(VH_VERIFY_RESOLVE(__VA_ARGS__, VH_VERIFY_MESSAGE, VH_VERIFY_NO_MESSAGE))

	#define VH_VERIFY(...) VH_EXPAND_VARGS( VH_GET_VERIFY_MACRO(__VA_ARGS__)(__VA_ARGS__))
	#define VH_CORE_VERIFY(...) VH_EXPAND_VARGS( VH_GET_VERIFY_MACRO(__VA_ARGS__)(__VA_ARGS__))
#else
	#define VH_VERIFY(...) 
	#define VH_CORE_VERIFY(...)
#endif // VH_ENABLE_VERIFY

