#pragma once

#include <SFML/Graphics.hpp>

#include "TextManager.hpp"

#include "Options.hpp"

#include "SoundManager.hpp"

#include "Szczur/Utility/Logger.hpp"
#include "Szczur/Utility/Convert/Hash.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/Script/Script.hpp"

#include "DialogGUI.hpp"

namespace rat {
    class DialogManager {
    public:
        using Options_t = std::vector<Options*>;

        using CharactersBinds_t = boost::container::flat_map<Hash32_t, sf::Texture*>;

        DialogManager(const std::string& path, DialogGUI& dialogGUI);

        ~DialogManager();

        static void initScript(Script& script);

        bool update(float deltaTime);

        void load(const std::string& path);

        void skip();

        void startWith(size_t major, size_t minor);

        void play(size_t major, size_t minor);
        void play();

        void bindCharacter(const std::string& character, const std::string& path);

        template<typename... Ts>
        void addOptions(Ts*... options) {
            (_options.push_back(options), ...);
        }

		bool isDialogPlaying();

    private:
        TextManager _textManager;
        SoundManager _soundManager;

        DialogGUI& _dialogGUI;

        Options_t _options;

        CharactersBinds_t _charactersBinds;

        size_t _currentMajor;
        size_t _currentMinor;

        bool _paused;
        bool _clearButtons;
        bool _destroyed{false};

		bool _isDialogPlaying = false;

        //bool _nextMinor();

        void _changeStruct(TextStruct* strct);
    };
}
