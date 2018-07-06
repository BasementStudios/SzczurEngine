#pragma once

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Listener/Listener.hpp"

#include "SoundEditor.hpp"
#include "MusicEditor.hpp"

namespace rat
{
    class AudioEditor : public Module <Window, Input, Music, Sound, Listener>
    {

    private:

        MusicEditor _musicEditor;
        SoundEditor _soundEditor;

        float _globalVolume {100};

        bool _showSoundEditor {false};
        bool _showMusicEditor {false};

    public:

        AudioEditor();

        void render();
    
    };

}