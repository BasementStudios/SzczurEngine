#pragma once

#include <boost/container/flat_map.hpp>

#include "DialogData.hpp"

namespace rat 
{
    class DLGTextManager 
    {
    
    public:

        using TextContainer_t = std::vector<std::vector<DialogData*>>; //TODO: Fuck off raw pointers!

    private:

        TextContainer_t _texts;

        std::vector<CharacterData>& _characters;

    public:

        DLGTextManager(std::vector<CharacterData>& characters);
        ~DLGTextManager();

        void save(const std::string& path);
        void load(const std::string& path);
        
        void add(const size_t key1, const size_t key2, DialogData* dialog);
        
        void remove(const size_t key1);
        void remove(const size_t key1, const size_t key2);

        TextContainer_t& getContainer();
    };
}