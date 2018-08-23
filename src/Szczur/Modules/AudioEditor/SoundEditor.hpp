#pragma once

#include <string>
#include <list>

#include "Szczur/Modules/Sound/Sound.hpp"

namespace rat
{
    class SoundEditor
    {

        using Container_t = std::list<SoundBase>;
    public:

        SoundEditor(Sound& sound, bool& showingEditor);

        void render();

        void save(Container_t::iterator it);

        void load();
        void add();

    private:

        std::string toTime(float secF);
        float toFloatSeconds(const std::string& timeString) const;
        void checkTimeString(std::string& timeString);

		void showEditor();

		Sound& _sound;
		SoundAssets _assets;

		bool& _isListDisplayed;
		bool _isLoadingDisplayed{ false };
		bool _isEditorDisplayed{ false };

		Container_t _soundHolder;

		Container_t::iterator _currentEditing;
    
    };
}