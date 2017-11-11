#pragma once

#if __has_include(<filesystem>)
#include <filesystem>
namespace filesystem = std::filesystem;
#elif __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
namespace filesystem = std::experimental::filesystem;
#elif __has_include(<boost/filesystem.hpp>)
#include <boost/filesystem.hpp>
namespace filesystem = boost::filesystem;
#else
#error No <filesystem> header is avaible
#endif
