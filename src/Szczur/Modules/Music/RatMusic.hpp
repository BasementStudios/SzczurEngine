#pragma once

#include <SFML/Audio/Music.hpp>

#include "Szczur/Modules/AudioEffects/AudioEffect.hpp"

#define MUSIC_DATA_FILE_PATH "Assets/Music/music.json"

namespace rat
{
	class RatMusic : public sf::Music, public AudioEffect
	{

	private:

		unsigned int _counter {0};

		std::string _filePath {""};
		std::string _name	  {""};

		float _bpm 		{60};
		float _fadeTime {0};

	public:

		RatMusic();

		bool load(const std::string& name);

		void incrementCounter();
		void decrementCounter();

		unsigned int getCounterValue() const;

		void saveToJson(); //Only for editor

		const std::string& getFilePath() const;
		const std::string& getName() const;

		float getBPM() const;
		void setBPM(float bpm);

		float getFadeTime() const;
		void setFadeTime(float fadeTime);

	private:

		void getJsonData();

	};
}