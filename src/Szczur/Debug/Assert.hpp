#pragma once

#ifndef NDEBUG

#include <cassert>

#define ASSERT(message, expr) assert(!message && (expr))

#else

#define ASSERT(...)

#endif
