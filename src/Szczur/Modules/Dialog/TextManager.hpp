#pragma once

#include <boost/container/flat_map.hpp>

#include <SFML/Graphics.hpp>

#include "Szczur/Utility/Hash.hpp"

#include "TextStruct.hpp"

namespace rat {
    class TextManager {
    public:
        using Type_t = TextStruct;
        using Key_t = size_t;
        template<typename T>
        using FlatMap_t = boost::container::flat_map<Key_t, T>;
        using TextContainer_t = FlatMap_t<FlatMap_t<Type_t*>>;

        TextManager();
        ~TextManager();

        void load(const std::string& path);

        Type_t* add(const Key_t key1, const Key_t key2, Type_t* dialog);
        void set(const Key_t key);
        void next();

        const std::string& setMinor(const Key_t key);
        const std::string& nextMinor();
        const std::string& getText() const;
        Type_t* getStruct() const;

        bool isMinorFinished();
    private:
        bool _finishedMinor;

        TextContainer_t _texts;
        TextContainer_t::iterator _current;
        TextContainer_t::mapped_type::iterator _minorCurrent;
    };
}