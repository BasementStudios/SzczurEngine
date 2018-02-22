#pragma once

#include <boost/container/flat_map.hpp>

#include <SFML/Graphics.hpp>

#include "Szczur/Utility/Hash.hpp"

#include "TextStruct.hpp"

namespace rat {
    class TextManager {
    public:
        using Key_t = size_t;
        using Type_t = TextStruct;
        using TextContainer_t = boost::container::flat_map<Key_t, Type_t*>;

        TextManager();
        ~TextManager();

        void load(const std::string& path);

        Type_t* add(const Key_t key, Type_t* dialog);
        const std::string& set(const Key_t key);
        const std::string& next();
        const std::string& getText() const;
        Type_t* getStruct() const;
    private:
        TextContainer_t _texts;
        TextContainer_t::iterator _current;
    };
}