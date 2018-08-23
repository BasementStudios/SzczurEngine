#pragma once

#include <string>
#include <list>

#include "RatMusic.hpp"

namespace rat
{

    class MusicAssets
    {

    private:

        std::list<RatMusic*> _musicHolder;

    public:

        ~MusicAssets();

        RatMusic* load(const std::string& name);
        void unload(RatMusic* music);

    };

}
