#pragma once

#ifndef NDEBUG

#include <cassert>

#define rat_Assert(expr) assert(expr)

#else

#define rat_Assert(...)

#endif
