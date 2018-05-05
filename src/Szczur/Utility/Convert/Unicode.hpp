#pragma once

#include <cstring>

#include <SFML/Graphics/String.hpp>

namespace rat
{

namespace detail
{

///
template <typename InputIt>
size_t getUnicodeBytesCount(InputIt begin, InputIt end);

///
template <typename InputIt, typename OutputIt>
void utf8ToUnicode(InputIt begin, InputIt end, OutputIt dst);

///
template <typename InputIt>
size_t getUtf8BytesCount(InputIt begin, InputIt end);

///
template <typename InputIt, typename OutputIt>
void unicodeToUtf8(InputIt begin, InputIt end, OutputIt dst);

}

///
sf::String getUnicodeString(const std::string& src);

///
std::string getUtf8String(const sf::String& src);

}

#include "Unicode.tpp"
