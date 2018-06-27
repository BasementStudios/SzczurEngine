#include "Unicode.hpp"

#include <SFML/System/String.hpp>

namespace rat
{

sf::String getUnicodeString(const std::string& src)
{
	std::basic_string<uint32_t> str;

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
