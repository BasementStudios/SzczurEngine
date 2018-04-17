#pragma once

#include <vector>
#include <string>

#include "DLGEditor.hpp"

namespace rat
{
    class CharactersManager
    {

    private:

        std::vector<std::string> _charactersName;
        std::vector<std::string> _charactersImagePath;

        DLGEditor::TextContainer_t& _dialogParts;

    public:

        CharactersManager(DLGEditor::TextContainer_t& dialogParts);

        void load(const std::string& path);
        void save(const std::string& path);

        void clear();

        void update();

        std::vector<std::string>& getNamesContainer();
        std::vector<std::string>& getImagePathsContainer();

    };
}