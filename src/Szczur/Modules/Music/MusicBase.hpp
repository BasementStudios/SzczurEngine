#pragma once

#include <string>
#include <memory>

#include "RatMusic.hpp"

namespace rat 
{
	class MusicBase
	{

	private:

		RatMusic& _base;
		
		std::string _name;
		float _bpm;
		float _timeLeft;
		float _fadeTime;
		float _baseVolume;

		bool _isEnding = false;
		bool _finishing = false;

		bool _finishInit = true;
		bool _startInit = true;

	public:

		MusicBase(const std::string& fileName, RatMusic& source);

		void update(float deltaTime);

		bool isEnding();
		bool finish(float deltaTime);
		void start(float deltaTime, float introTime);

		void play();
		void pause();
		void stop();

		void setLoop(bool loop);
		bool getLoop() const;

		float getVolume() const;
		void setVolume(float volume);

		sf::SoundSource::Status getStatus() const;

		float getFadeTime() const;
		float getDuration() const;

		float getTimeLeft() const;
		void setTimeLeft(float timeLeft);

		const std::string& getName() const;

		RatMusic& getSource() const;
		
		void reset();

	private:

		void loadMusic();
		void getJsonData();

		const std::string& getPath() const;

	};
}