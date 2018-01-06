#include "Unicode.hpp"

sf::String getUnicodeString(const std::string& src)
{
	std::basic_string<uint32_t> str;

	str.reserve(detail::getUnicodeBytesCount(src.begin(), src.end()));

	detail::utf8ToUnicode(src.begin(), src.end(), std::back_inserter(str));

	return str;
}
