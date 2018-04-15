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

        void load(const std::string& path);
        void save(const std::string& path);

        void update();

        std::vector<std::string>& getNamesContainer();
        std::vector<std::string>& getImagePathsContainer();

    };
}