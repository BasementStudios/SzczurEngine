#pragma once

#include <unordered_map>

#include "SoundBuffer.hpp"

namespace rat
{

    class SoundAssets
    {
        
        using Container_t = std::unordered_map<std::string, SoundBuffer>;

    private:

        Container_t _buffersHolder;

    public:

        bool load(const std::string& fileName);
        void unload(SoundBuffer* buffer);

        SoundBuffer* get(const std::string& fileName);

    };

}
