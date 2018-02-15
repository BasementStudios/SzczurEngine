#pragma once

#if defined(__GNUG__)
#	define COMPILER_GCC
#elif defined(__clang__)
#	define COMPILER_CLANG
#elif defined(_MSC_VER)
#	define COMPILER_MSVC
#else
#	error Compiler not supported!
#endif

#if defined(COMPILER_MSVC)
#	define DEPENDENT_TEMPLATE_SCOPE
#	define DIRECTORY_SEPARATOR_CHAR '\\'
#else
#	define DEPENDENT_TEMPLATE_SCOPE template
#	define DIRECTORY_SEPARATOR_CHAR '/'
#endif
