#pragma once

#include "DLGEditor.hpp"

namespace rat
{
    class CharactersManager
    {
    private:

        DLGEditor::TextContainer_t& _dialogParts;

        std::vector<CharacterData> _characters;

    public:

        CharactersManager(DLGEditor::TextContainer_t& dialogParts);

        void load(const std::string& path);
        void save(const std::string& path);

        void clear();

        void update();

        std::vector<CharacterData>& getCharactersContainer();

    };
}