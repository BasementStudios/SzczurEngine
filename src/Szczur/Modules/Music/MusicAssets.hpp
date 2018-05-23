#pragma once

#include <unordered_map>

#include "RatMusic.hpp"

namespace rat
{

    class MusicAssets
    {
        
        using Container_t = std::unordered_map<std::string, RatMusic>;

    private:

        Container_t _musicHolder;

        const std::string DEFAULT_PATH;

    public:

        MusicAssets(const std::string& path);

        void load(const std::string& name);
        void unload(const std::string& name);

        RatMusic& get(const std::string& name);

    private:

        std::string getPath(const std::string& fileName) const;

    };

}
