
#include <string>

namespace rat
{

inline std::string stringReplaced(const std::string& in, const std::string& from, const std::string& to)
{
	std::string string = in;
	
	for (
		std::size_t i = string.find(from); 
		i != std::string::npos; 
		i = string.find(from, i)
	) {
		string.replace(i, from.length(), to);
	}

	return string;
}

}
