#pragma once

#ifndef PLAYGROUND_SRC
    #error "Missing define(s): PLAYGROUND_SRC"
#endif

#include <cstdlib>
#include <cstddef>
#include <cstdint>
#include <cassert>

#define OK   EXIT_SUCCESS;
#define FAIL EXIT_FAILURE;

#define never if (0)
#define until(...) while (!(__VA_ARGS__))
#define loop while (true)

using ierr = int;
using umin = unsigned char;
using imin = signed char;
using umax = uintmax_t;
using imax = intmax_t;
using uptr = uintptr_t;
using iptr = intptr_t;
using usize = size_t;
using isize = ptrdiff_t;
using uword = unsigned long;
using iword = signed long;
using ufast = unsigned int;
using ifast = signed int;

using u0 = void;

using i8 = int8_t;
using u8 = uint8_t;

using i16 = int16_t;
using u16 = uint16_t;

using i32 = int32_t;
using u32 = uint32_t;

using i64 = int64_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;

using octet = u8;
using byte = octet;
using rune = u32;
