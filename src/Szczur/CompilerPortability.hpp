#pragma once

#if defined(__GNUG__)
#define COMPILER_GCC
#elif defined(__clang__)
#define COMPILER_CLANG
#elif defined(_MSC_VER)
#define COMPILER_MSVC
#else
#error Compiler not supported!
#endif

#if defined(COMPILER_MSVC)
#define DEPENDENT_TEMPLATE_SCOPE
#else
#define DEPENDENT_TEMPLATE_SCOPE template
#endif
