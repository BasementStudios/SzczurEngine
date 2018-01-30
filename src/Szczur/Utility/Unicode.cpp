#include "Unicode.hpp"

sf::String getUnicodeString(const char* src)
{
	std::basic_string<uint32_t> str;

	auto len = std::strlen(src);

	str.reserve(detail::getUnicodeBytesCount(src, src + len));

	detail::utf8ToUnicode(src, src + len, std::back_inserter(str));

	return str;
}

sf::String getUnicodeString(const std::string& src)
{
	std::basic_string<uint32_t> str;

	str.reserve(detail::getUnicodeBytesCount(src.begin(), src.end()));

	detail::utf8ToUnicode(src.begin(), src.end(), std::back_inserter(str));

	return str;
}

sf::String getUnicodeString(std::string_view src)
{
	std::basic_string<uint32_t> str;

	str.reserve(detail::getUnicodeBytesCount(src.begin(), src.end()));

	detail::utf8ToUnicode(src.begin(), src.end(), std::back_inserter(str));

	return str;
}
