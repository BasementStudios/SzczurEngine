#pragma once

#include <SFML/Audio.hpp>

#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Config.hpp"

#include "DLGTextManager.hpp"

namespace rat
{

    class DLGEditor
    {
	public:
        using TextContainer_t = boost::container::flat_map<size_t, boost::container::flat_map<size_t, DialogData*>>;

	private:

        const InputManager& _inputManager;

        std::vector<CharacterData>& _characters;
        DLGTextManager _textManager;
        TextContainer_t& _parts;

        size_t _currentMajor = 1;
        size_t _currentMinor = 1;

        std::string _dialogPath;

        sf::SoundBuffer _buffer;
        sf::Sound       _dialogAudio;
        float           _audioDuration;

        bool _playing  = false;
        bool _playTest = false;

        int _dialogPlayerEnd = 0;

        std::string _sliderTimeString = "";
    
        int _startTestPlayerOffset;
        int _endTestPlayerOffset;

        int _startPlayerOffset;
        int _endPlayerOffset;

        float _sliderTime = 0;

        bool _isWindowFocused;

    public:

        DLGEditor(std::vector<CharacterData>& characters, const InputManager& inputManager);
        void load(const std::string& path);

        void update();

        void loadData();
        void save();

        TextContainer_t& getContainer();

		void setCurrentMajor(int major);

    private:

        void input();

        void playAudio();
        void show();

        void player();
        void majorPartSelector();
        void minorPartSelector();
        void timeSelector();
        void labelEditor();
        void mainEditor();

        std::string toTime(int secI);
        int toIntSeconds(const std::string& timeString) const;

        void checkTimeString(std::string& timeString);

        bool charactersCombo(int* currentItem) const;

    };
}
