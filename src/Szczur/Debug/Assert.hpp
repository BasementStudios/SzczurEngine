#pragma once

#ifndef NDEBUG

#include <cassert>

#define rat_Assert(code) assert(code)

#else

#define rat_Assert(...)

#endif
