// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <cassert>
#include <cstddef>
#include <limits>


// TODO: reference additional headers your program requires here

template <typename ContainerType>
/* constexpr */ int size_as_int(const ContainerType &c) {
	const auto size = c.size();  // if no auto, use `typename ContainerType::size_type`
	assert(size <= static_cast<std::size_t>(std::numeric_limits<int>::max()));
	return static_cast<int>(size);
}