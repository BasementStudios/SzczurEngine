#pragma once

#if __has_include(<cxxabi.h>)
#include <cxxabi.h>
#include <memory>
#endif
#include <string>
#include <typeinfo>

namespace rat
{

template <typename T>
inline std::string DemangleType()
{
	#if __has_include(<cxxabi.h>)
	return std::unique_ptr<char, void(*)(void*)>{ abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr), std::free }.get();
	#else
	return typeid(T).name();
	#endif
}

}
