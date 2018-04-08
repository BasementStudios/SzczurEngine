#pragma once

#include <boost/container/flat_map.hpp>
#include <vector>
#include "DLGTextManager.hpp"
#include <SFML/Audio.hpp>

#include "Szczur/Config.hpp"

namespace rat
{

    class DLGEditor
    {
        using TextContainer_t = boost::container::flat_map<size_t, boost::container::flat_map<size_t, DialogData*>>;

        std::vector<std::string>& _characters;
        DLGTextManager _textManager;
        TextContainer_t& _parts;

        size_t _currentMajor;
        size_t _currentMinor;

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

    public:

        DLGEditor(std::vector<std::string>& characters);
        void load(const std::string& path);

        void update();

        TextContainer_t& getContainer();

    private:

        void playAudio();
        void show();

        void loadData();

        void player();
        void majorPartSelector();
        void minorPartSelector();
        void timeSelector();
        void labelEditor();
        void mainEditor();
        void saveAndReloadButton();

        std::string toTime(int secI);
        int toIntSeconds(const std::string& timeString);

        bool combo(const char* label, int* currentItem, const std::vector<std::string>& items, int itemsCount, int heightInItems = -1);

    };
}
