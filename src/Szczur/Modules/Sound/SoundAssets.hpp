#pragma once

#include <unordered_map>

#include "SFML/Audio/SoundBuffer.hpp"

namespace rat
{

    class SoundAssets
    {
        
        using Container_t = std::unordered_map<std::string, sf::SoundBuffer>;

    private:

        Container_t _buffersHolder;

    public:

        void load(const std::string& fileName);
        void unload(const std::string& fileName);

        sf::SoundBuffer& get(const std::string& fileName);

    };

}
