#pragma once

#include <SFML/Graphics/Text.hpp>

namespace detail
{

template<typename InputIt>
size_t getUnicodeBytesCount(InputIt begin, InputIt end);

template<typename InputIt, typename OutputIt>
void utf8ToUnicode(InputIt begin, InputIt end, OutputIt dst);

}

void setUnicodeText(sf::Text& dst, const std::string& src);

#include "Unicode.tpp"
