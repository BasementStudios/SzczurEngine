#pragma once

#include <vector>

#include "CharacterData.hpp"

namespace rat 
{
    struct DialogData
    {
        size_t id;
        size_t minorId;

        std::string audioStartTime;
        std::string audioEndTime;
    
        std::string label;
        
        std::vector<std::string> dialogs;
        std::vector<std::string> dialogTime; 
        std::vector<int>         chosenCharacter;

        unsigned int dialogLines = 0;

        void interpretText(const std::string& text, std::vector<CharacterData>& characters);
    };
}