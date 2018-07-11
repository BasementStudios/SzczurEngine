#pragma once

#include <string>
#include <list>
#include <vector>

#include "Szczur/Modules/Sound/Sound.hpp"

namespace rat
{
    class SoundEditor
    {

        using Container_t = std::list<SoundBase>;
    
    private:

        Sound& _sound;
        SoundAssets _assets;
        
        bool _editor       {false};
        bool _loadingSound {false};

        Container_t _soundHolder;

        Container_t::iterator _currentEditing;

        std::vector<std::string> _loadingNames;
        bool* _choosedNames;
 
    public:

        SoundEditor(Sound& sound);

        void render();

        void save(Container_t::iterator it);

        void load();
        void add();

    private:

        void updateSoundHolder();

        void getJsonData();

        std::string toTime(float secF);
        float toFloatSeconds(const std::string& timeString) const;
        void checkTimeString(std::string& timeString);
    
    };
}