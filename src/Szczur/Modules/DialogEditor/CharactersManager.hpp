#pragma once

#include <vector>
#include <string>

namespace rat
{
    class CharactersManager
    {

    private:

        std::vector<std::string> _charactersName;
        std::vector<std::string> _charactersImagePath;

    public:

        CharactersManager();

        void update();

        std::vector<std::string>& getNamesContainer();
        std::vector<std::string>& getImagePathsContainer();

    private:

        void load();
        void save();

    };
}