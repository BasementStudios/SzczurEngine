#pragma once

#include <vector>

#include "CharacterData.hpp"

namespace rat 
{
    struct DialogData
    {
        size_t id;
        std::string majorName;
        
        size_t minorId;
        std::string minorName;

        std::string audioStartTime;
        std::string audioEndTime;
    
        std::string label;
        
        std::vector<std::string> dialogs;
        std::vector<std::string> dialogTime; 
        std::vector<int>         chosenCharacter;

        unsigned int dialogLines = 0;

        std::string majorFullName;
        std::string minorFullName;

        void interpretText(const std::string& text, std::vector<CharacterData>& characters);
        
        void renameMajor(const std::string& name);
        void renameMinor(const std::string& name);
    };
}