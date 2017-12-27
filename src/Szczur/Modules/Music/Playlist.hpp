#pragma once

#include <vector>
#include <string>
#include <memory>

#include <boost/container/flat_map.hpp>

#include "MusicBase.hpp"

namespace rat 
{
    class Playlist 
	{ 
		using Container_t = boost::container::flat_map<std::string, std::shared_ptr<MusicBase>>;

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
		Container_t::iterator _currentPlaying;

		Container_t::iterator _endingFile;
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

		bool loadMusic(const std::string& fileName);

		void playNext();

		std::string getNameOfRandomFile() const;

		void unPause();

	};
}



















