#include "AssetNotFoundException.hpp"

/** @file AssetNotFoundException.cpp
 ** @description Implementaion file of AssetNotFoundException class functions. 
 ** @auhtor Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>                       // string, npos
#include <string_view>                  // string_view

namespace rat
{

std::string_view AssetNotFoundBasicException::selector() const
{
    std::size_t start = _message.find(AssetNotFoundBasicException::_string_selector);
    if(start != std::string::npos) {
        start += AssetNotFoundBasicException::_string_selector_length;
        std::size_t end = _message.find('\n');
        if(end == std::string::npos) {
            return std::string_view(_message.data() + start);
        } else {
            return std::string_view(_message.data() + start, end - start);
        }
    }
    return std::string_view();
}

std::string_view AssetNotFoundBasicException::type() const 
{
    std::size_t start = _message.find(AssetNotFoundBasicException::_string_type);
    if(start != std::string::npos) {
        start += AssetNotFoundBasicException::_string_type_length;
        std::size_t end = _message.find('\n');
        if(end == std::string::npos) {
            return std::string_view(_message.data() + start);
        } else {
            return std::string_view(_message.data() + start, end - start);
        }
    }
    return std::string_view();
}

}
