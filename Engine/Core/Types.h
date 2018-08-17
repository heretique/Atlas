#pragma once

//#include <cinttypes>
#include <cstddef>
#include <cstdint>
#include <limits>

// Types
using uint = unsigned int;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using uptr = uintptr_t;

static const size_t kInvalidPackedIndex = std::numeric_limits<size_t>::max();
