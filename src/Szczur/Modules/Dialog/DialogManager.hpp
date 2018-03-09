#pragma once

#include "TextManager.hpp"
#include "SoundManager.hpp"

#include "Options.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"

#include "DialogGUI.hpp"

namespace rat {
    class DialogManager {
    public:
        using Options_t = std::vector<Options*>;

        DialogManager(const std::string& path, DialogGUI& dialogGUI);

        ~DialogManager();

        void update(float deltaTime);

        void load(const std::string& path);

        void startWith(size_t id);

        void play(size_t id);
        void play();

        

        template<typename... Ts>
        void addOptions(Ts*... options) {
            (_options.push_back(options), ...);
        }

    private:
        TextManager _textManager;
        SoundManager _soundManager;

        DialogGUI& _dialogGUI;

        Options_t _options;

        size_t _current;

        bool _paused;
        bool _clearButtons;

        bool _nextMinor();

        void _changeStruct(TextStruct* strct);
    };
}