#pragma once

#if __has_include(<Json/json.hpp>)
#	include <Json/json.hpp>
	using json = nlohmann::json;
#elif __has_include(<json.hpp>)
#	include <json.hpp>
	using json = nlohmann::json;
#else
#	error No <json.hpp> header is avaible
#endif

#include <boost/filesystem.hpp>
namespace std {
namespace filesystem = boost::filesystem;
}
