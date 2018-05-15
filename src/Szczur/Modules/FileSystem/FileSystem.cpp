#include "FileSystem.hpp"

namespace rat::file
{

time_t getLastModificationTime(const std::string& path)
{
#ifdef OS_WINDOWS
	struct _stat result;
	if (_stat(path.c_str(), &result) == 0)
	{
		return result.st_mtime;
	}
#elif
	return 0;
#endif
}

time_t getCreationTime(const std::string& path)
{
#ifdef OS_WINDOWS
	struct _stat result;
	if (_stat(path.c_str(), &result) == 0)
	{
		return result.st_ctime;
	}
#elif
	return 0;
#endif
}

}
