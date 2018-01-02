#pragma once

#include <vector>
#include <string>
#include <memory>

#include "MusicBase.hpp"

namespace rat 
{
    class Playlist 
	{ 
		using Container_t = std::vector<std::shared_ptr<MusicBase>>;

    public:

		enum class PlayingMode {
			Random,
			Orderly,
			Single
		};

	private:

		enum class Status {
			Playing, 
			Paused,
			Stopped
		};

		Container_t _playlist;
		unsigned int _currentID;

		unsigned int _endingID;
		bool _isFileEnding = false;

		bool hasBeenEverPlayed = false;

		PlayingMode _playingMode = PlayingMode::Random;
		Status _status;

		inline static float _globalVolume = 100;

	public:

		Playlist(std::vector<std::string> newPlaylist);
	
		bool setNewPlaylist(std::vector<std::string> newPlaylist);
		void clear();

		void update(float deltaTime);

		bool includes(const std::string& fileName) const;		

		bool add(const std::string& fileName);
		void remove(const std::string& fileName);

		void play(const std::string& fileName = "");
		void pause();

		void setPlayingMode(PlayingMode mode);

		void setVolume(float volume, const std::string& fileName = "");

	private:
		void play(unsigned int id);

		bool loadMusic(const std::string& fileName);

		void playNext();

		void unPause();

		unsigned int getID(const std::string& fileName) const;
		unsigned int getRandomId() const;
			
	};
}



















