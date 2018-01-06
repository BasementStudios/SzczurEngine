#pragma once

/** @file AssetNotFoundException.hpp
 ** @description Header file with AssetNotFoundException class. 
 ** @auhtor Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>                       // string, npos
#include <string_view>                  // string_view
#include <exception>                    // exception

#include "AssetTraits.hpp"

namespace rat {

/** @class AssetNotFoundBasicException
 ** @inherits std::exception
 ** @description Occurs when asset cannot be find.
 **/
class AssetNotFoundBasicException : public std::exception
{
    static constexpr const char* _string_base = "Asset not found.";
    static constexpr const char* _string_selector = "\nSelector: ";
    static constexpr const char* _string_type     = "\nType:     ";
    static constexpr const std::size_t _string_base_length     = 16;
    static constexpr const std::size_t _string_selector_length = 11;
    static constexpr const std::size_t _string_type_length     = 11;
    
    /* Variables */
protected:
    std::string _message;

    /* Operators */
public:
    explicit AssetNotFoundBasicException(const std::string& selector = "", const std::string& type = "") :
        _message(_string_base
            + (    type.empty() ? _string_type     + type     : "")
            + (selector.empty() ? _string_selector + selector : ""))
    {}
    
    /* Methods */
public:
    virtual const char* what() const noexcept {
        return _message.c_str();
    }

    /// Returns selector field of exception message. 
    std::string_view selector() const;

    /// Returns type field of exception message. 
    std::string_view type() const;
};

/** @class AssetNotFoundException
 ** @inherits std::exception
 ** @description Occurs when asset cannot be find.
 **/
template <typename TType>
class AssetNotFoundException : public AssetNotFoundBasicException {
public:
    explicit AssetNotFoundException(const std::string& selector = "") : 
        AssetNotFoundBasicException(selector, AssetTraits<TType>::string)
    {}
};

}
