#pragma once

#include <string>

#include <SFML/Audio/Music.hpp>

namespace rat 
{
	class MusicBase 
	{

	private:

		float _bpm;
		float _timeLeft;
		float _postTime;
		float _baseVolume;
		bool _isEnding = false;
		std::string _name;
		sf::Music _base;

	public:

		bool init(const std::string& fileName, float volume);
		void update(float deltaTime);

		bool isEnding();
		bool finish(float deltaTime);
		void start(float deltaTime, float preTime);

		void play();
		void pause();
		void stop();

		void setLoop(bool loop);
		bool getLoop() const;

		float getVolume() const;
		void setVolume(float volume);

		sf::SoundSource::Status getStatus() const;

		float getPostTime() const;
		float getDuration() const;

		const std::string& getName() const;
		
	private:

		void reset();

		bool loadMusic();
		void getJsonData();

		std::string getPath() const;

	};
}