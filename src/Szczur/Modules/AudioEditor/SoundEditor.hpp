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
    public:

        SoundEditor(Sound& sound, bool& showingEditor);
		~SoundEditor();

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

		float arr[2] = { 0.f, 10.f }; //temp
		float timestamp = 0.f;//temp
		char* path = new char[64];
		bool isPlaying = false;

		Container_t _soundHolder;
		std::vector<std::string> _soundNames;

		Container_t::iterator _currentEditing;
    
    };
}