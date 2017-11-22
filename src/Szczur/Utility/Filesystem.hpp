#pragma once

#if __has_include(<filesystem>)
	#include <filesystem>
#elif __has_include(<experimental/filesystem>)
	#include <experimental/filesystem>
	namespace std {
		namespace filesystem = std::experimental::filesystem;
	}
#elif __has_include(<boost/filesystem.hpp>)
	#include <boost/filesystem.hpp>
	namespace std {
		namespace filesystem = boost::filesystem;
	}
#else
	#error No <filesystem> header is avaible
#endif
