#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Szczur/Modules/AudioEffects/AudioEffects.hpp"

#include "MusicBase.hpp"
#include "MusicAssets.hpp"

namespace rat 
{
	class Playlist
	{ 

	public:

		using BasePointer_t = std::shared_ptr<MusicBase>;
		using Container_t = std::vector<BasePointer_t>;

		enum class PlayingMode {
			Random,
			Orderly,
			Single
		};

		enum class Status {
			Playing, 
			Paused,
			Stopped
		};

	private:

		Container_t _playlist;

		unsigned int _currentID = 0;

		BasePointer_t _endingFile = nullptr;
		bool _isFileEnding = false;

		bool _hasBeenEverPlayed = false;

		PlayingMode _playingMode = PlayingMode::Random;
		Status      _status      = Status::Stopped;

		AudioEffects& _effects;

	public:

		Playlist(AudioEffects& effects);

		void clear();

		void update(float deltaTime);
		void stopUpdates();

		bool includes(const std::string& name) const;		

		void add(MusicBase&& base);
		void remove(const std::string& name);

		BasePointer_t getCurrentPlaying() const;

		void play(unsigned int id, float timeLeft);
		void play(const std::string& name = "");
		void play(BasePointer_t prevMusicFile, const std::string& name = "");

		void pause();
		void stop();

		void setPlayingMode(PlayingMode mode);

		Status getStatus() const;

		void setVolume(float volume, const std::string& name = "");
		float getVolume(const std::string& name) const;

		unsigned int getID(const std::string& name) const;

		const Container_t& getContainerRef() const;

		void setGlobalEffects();

	private:

		void play(unsigned int id);

		void playNext();

		void unPause();

		unsigned int getRandomId() const;
		
		void setPlaylistToPlaying(int id);
			
	};

}