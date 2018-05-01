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
        using TextContainer_t  = DLGTextManager::TextContainer_t;

	private:

        const InputManager& _inputManager;

        std::vector<CharacterData>& _characters;
        DLGTextManager _textManager;
        TextContainer_t& _parts;

        int _currentMajor = 0;
        int _currentMinor = 0;

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

        bool _isRenamingWindowFocused;
        bool _renaming;
        enum {
            Minor,
            Major
        } _renameType;
        int _renamingMajor;
        int _renamingMinor;
        std::string _renamingName;

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

        void addMinor();
        void removeMinor();

        void rename();

        std::string toTime(int secI);
        int toIntSeconds(const std::string& timeString) const;

        void checkTimeString(std::string& timeString);

        bool charactersCombo(int* currentItem) const;
        bool majorsCombo(); 
        bool minorsCombo();

    };
}
