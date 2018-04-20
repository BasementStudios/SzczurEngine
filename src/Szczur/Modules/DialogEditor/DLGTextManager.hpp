#pragma once

#include <boost/container/flat_map.hpp>

#include "DialogData.hpp"

namespace rat 
{
    class DLGTextManager 
    {
        template<typename T>
        using FlatMap_t = boost::container::flat_map<size_t, T>;
        using TextContainer_t = FlatMap_t<FlatMap_t<DialogData*>>; //TODO: Fuck off raw pointers!

        TextContainer_t _texts;

        std::vector<CharacterData>& _characters;

    public:

        DLGTextManager(std::vector<CharacterData>& characters);
        ~DLGTextManager();

        void save(const std::string& path);
        void load(const std::string& path);
        
        void add(const size_t key1, const size_t key2, DialogData* dialog);

        TextContainer_t& getContainer();
    };
}