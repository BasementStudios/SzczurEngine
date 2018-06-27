#pragma once

#include "Szczur/Modules/Sound/SoundManager.hpp"

namespace rat
{
    class SoundEditor
    {
    
    private:

        SoundManager& _soundManager;
        
        bool _editor      {false};
        bool _addingSound {false};

        std::string _currentEditing {""};

        std::vector<std::string> _soundNames;
 
    public:

        SoundEditor(SoundManager& soundManager);

        void render();

        void save(const std::string& fileName);

        void load();
        void add();

    private:

        std::string toTime(float secF);
        float toFloatSeconds(const std::string& timeString) const;
        void checkTimeString(std::string& timeString);
    
    };
}