#include "Common.h"

#include <fstream>


bool isFileExist(std::string_view filePath)
{
	std::ifstream f(filePath.data());
	return f.good();
}
