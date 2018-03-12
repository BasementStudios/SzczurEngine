#include "Unicode.hpp"

namespace rat
{

sf::String getUnicodeString(const std::string& src)
{
	std::basic_string<unsigned> str;

	str.reserve(detail::getUnicodeBytesCount(src.begin(), src.end()));

	detail::utf8ToUnicode(src.begin(), src.end(), std::back_inserter(str));

	return str;
}

std::string getUtf8String(const sf::String& src)
{
	std::string str;

	str.reserve(detail::getUtf8BytesCount(src.begin(), src.end()));

	detail::unicodeToUtf8(src.begin(), src.end(), std::back_inserter(str));

	return str;
}



}
