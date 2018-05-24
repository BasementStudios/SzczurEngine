#pragma once

// OS detection
#if defined(_WIN32)
#	define OS_WINDOWS
#	define OS_NAME "Windows"
#elif defined(__linux__)
#	define OS_LINUX
#	define OS_NAME "Linux"
#else
#	error Platform not supported!
#endif

// Compiler detection
#if defined(__clang__)
#	define COMPILER_CLANG
#	define COMPILER_NAME "LLVM/Clang"
#elif defined(__GNUG__)
#	define COMPILER_GCC
#	define COMPILER_NAME "GNU Compiler Collection"
#elif defined(_MSC_VER)
#	define COMPILER_MSVC
#	define COMPILER_NAME "Microsoft Visual C++"
#else
#	error Compiler not supported!
#endif

// Fixes for MSVC compiler
#if defined(COMPILER_MSVC)
#	define DEPENDENT_TEMPLATE_SCOPE
#	define DIRECTORY_SEPARATOR_CHAR '\\'
#else
#	define DEPENDENT_TEMPLATE_SCOPE template
#	define DIRECTORY_SEPARATOR_CHAR '/'
#endif

// Mode detection
#if defined(NDEBUG)
#	define RELEASE
#	define MODE_NAME "Release"
#else
#	define DEBUG
#	define MODE_NAME "Debug"
#endif

// Editor specification
#if defined(DEBUG) && defined(OS_WINDOWS)
#	define EDITOR
#endif

// Global helper ptr
namespace rat::detail { template <typename T> inline T* globalPtr = nullptr; }
