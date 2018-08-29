#pragma once
 
#include <string>
 
#include <SFML/Audio/Music.hpp>
 
#include "Szczur/Modules/AudioEffects/AudioEffect.hpp"

#define MUSIC_DATA_FILE_PATH "Assets/Music/music.json"

namespace rat
{
	class RatMusic : public sf::Music, public AudioEffect
	{

	private:

		inline static float MusicVolume {100};

		std::string _filePath {""};
		std::string _name	  {""};

		float _bpm 		{60};
		float _fadeTime {0};
		float _volume   {100};

	public:

		RatMusic();

		bool load(const std::string& name);

		void saveToJson(); //Only for editor

		const std::string& getFilePath() const;
		const std::string& getName() const;

		float getBPM() const;
		void setBPM(float bpm);

		float getFadeTime() const;
		void setFadeTime(float fadeTime);
		void setFadeBars(float bars);

		static float GetMusicVolume();
        static void SetMusicVolume(float volume);

		float getVolume() const;
		void setVolume(float volume);

	private:

		void getJsonData();

	};
}