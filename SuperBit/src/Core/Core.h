#pragma once

#include <memory>

#ifdef SB_DEBUG
#define SB_ENABLE_ASSERTS
#endif

#ifdef SB_ENABLE_ASSERTS
#define SB_ASSERT(x, ...) { if(!(x)) { SB_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define SB_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define SB_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Hazel {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}