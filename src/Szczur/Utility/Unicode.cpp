#include "Unicode.hpp"

void setUnicodeText(sf::Text& dst, const std::string& src)
{
	auto& ref = reinterpret_cast<std::basic_string<uint32_t>&>(const_cast<sf::String&>(dst.getString()));

	ref.clear();
	ref.reserve(detail::getUnicodeBytesCount(src.begin(), src.end()));

	detail::utf8ToUnicode(src.begin(), src.end(), std::back_inserter(ref));
}
