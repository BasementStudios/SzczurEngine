#pragma once

#include <cstring>
#include <string_view>

#include <SFML/Graphics/Text.hpp>

namespace detail
{

template <typename InputIt>
size_t getUnicodeBytesCount(InputIt begin, InputIt end);

template <typename InputIt, typename OutputIt>
void utf8ToUnicode(InputIt begin, InputIt end, OutputIt dst);

}

sf::String getUnicodeString(const char* src);
sf::String getUnicodeString(const std::string& src);
sf::String getUnicodeString(std::string_view src);

#include "Unicode.tpp"
