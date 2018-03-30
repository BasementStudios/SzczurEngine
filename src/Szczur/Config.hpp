#pragma once

// OS detection
#if defined(_WIN32)
#	define OS_WINDOWS
#elif defined(__linux__)
#	define OS_LINUX
#else
#	error Platform not supported!
#endif

// Compiler detection
#if defined(__GNUG__)
#	define COMPILER_GCC
#elif defined(__clang__)
#	define COMPILER_CLANG
#elif defined(_MSC_VER)
#	define COMPILER_MSVC
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

// Target detection
#if defined(NDEBUG)
#	define RELEASE
#else
#	define DEBUG
#endif

// Editor specification
#if defined(DEBUG) || defined(OS_WINDOWS)
#	define EDITOR
#	include <boost/lexical_cast.hpp>
#	include <ImGui/imgui.h>
#	include <ImGui/imgui-SFML.h>
#	include "Szczur/Utility/Debug/GlobalVarsHolder.hpp"
#endif

// Global helper for modules system
namespace rat::detail { template <typename T> inline T* globalPtr_v = nullptr; }
