#pragma once
#include <cstddef>
#include <new>

namespace ll::config
{
	inline constexpr std::size_t kCacheLineSize = std::hardware_destructive_interference_size;
}