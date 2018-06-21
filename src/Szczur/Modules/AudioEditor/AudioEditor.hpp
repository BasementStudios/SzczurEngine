#pragma once

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Sound/SoundManager.hpp"
#include "Szczur/Modules/Music/Music.hpp"

#include "SoundEditor.hpp"
#include "MusicEditor.hpp"

#include <ImGui/imgui.h>
#include <ImGui/imgui-SFML.h>

#include <fstream>

namespace rat
{
    class AudioEditor : public Module <Window, Input, Music>
    {
        SoundEditor _soundEditor;
        MusicEditor _musicEditor;

        const int BUFFER = 256;  

        bool deleteField = false;

        char file[256]          = {};
        char soundName[256]     = {};
        char saveFileName[256]  = {};
        char loadFileName[256]  = {};

        bool _showSoundEditor {false};
        bool _showMusicEditor {false};

    public:

        AudioEditor();

        void render();

        void addSound(const std::string& soundName, const std::string& soundFileName);
        
        void save(const std::string& fileName);
        void load(const std::string& fileName);
    
    };

}