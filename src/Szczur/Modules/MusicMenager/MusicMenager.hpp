#pragma once

#include <vector>
#include <string>

#include <SFML/Audio/Music.hpp>
#include <boost/container/flat_map.hpp>

#include "Szczur/Utility/Modules.hpp"

namespace rat {
    class MusicMenager : public ModuleBase<> { 
		using ModuleBase::ModuleBase;
		using Playlist = boost::container::flat_map<std::string, sf::Music*>;

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

		Playlist _playlist;
		Playlist::iterator _currentPlaying;
		PlayingMode _playingMode = PlayingMode::Orderly;
		Status _status = Status::Stopped;
		float _timeLeft;
		float _globalVolume = 100;

	public:

		void init();
		void input(const sf::Event& event);
		void render();
		void update(float deltaTime);
		
		bool setNewPlaylist(std::vector<std::string> newPlaylist);
		bool addToPlaylist(const std::string& fileName);
		void removeFromPlaylist(const std::string& fileName);
		void clearPlaylist();
		void play(const std::string& fileName = "");
		void pause();
		void setPlayingMode(PlayingMode mode);
		void setVolume(float volume, const std::string& fileName = "");
		bool isInPlaylist(const std::string& fileName) const;

	private:

		bool loadMusicByName(const std::string& fileName);
		void playNextMusicFile();
		std::string getPath(const std::string& fileName) const;
		std::string getNameRandomMusicFile() const;
	};
}



















