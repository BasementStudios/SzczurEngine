#include "MusicMenager.hpp"

#include <random>
#include <chrono>
#include <algorithm>

#include <iostream>

namespace rat {

	void MusicMenager::init() {

	}

	void MusicMenager::input(const sf::Event& event) {

	}

	void MusicMenager::render() {

	}

	void MusicMenager::update(float deltaTime) {
		if(_status == Status::Playing) {
			_timeLeft -= deltaTime;
			if(_timeLeft <= 0)
				playNextMusicFile();
		}
	}

	bool MusicMenager::addToPlaylist(const std::string& fileName) {
		return (isInPlaylist(fileName) || loadMusicByName(fileName));
	}

	void MusicMenager::removeFromPlaylist(const std::string& fileName) {
		if (_currentPlaying->first == fileName) {
			_status = Status::Stopped;
			_currentPlaying->second->stop();
		}
		_playlist.erase(fileName);
	}

	void MusicMenager::play(const std::string& fileName) {
		if (_status == Status::Playing)
			_currentPlaying->second->stop();

		if (fileName == "") {
			if (_playingMode == PlayingMode::Random)
				_currentPlaying = _playlist.find(getNameRandomMusicFile());
			else {
				_currentPlaying = _playlist.begin();
			} 
		}
		else if (isInPlaylist(fileName)) {
			_currentPlaying = _playlist.find(fileName);
		}

		std::cout << _currentPlaying->first << std::endl;
		_timeLeft = _currentPlaying->second->getDuration().asSeconds();
		_currentPlaying->second->play();
		_status = Status::Playing;
	}

	bool MusicMenager::setNewPlaylist(std::vector<std::string> newPlaylist) {	
		clearPlaylist();
		std::reverse(newPlaylist.begin() , newPlaylist.end());
		for (auto it : newPlaylist) {
			if(!loadMusicByName(it))
				return false;
		}
		for (auto it = _playlist.begin(); it != _playlist.end(); ++it) {
			std::cout << it->first << std::endl;
		}
		return true;
	}

	void MusicMenager::clearPlaylist() {
		_status = Status::Stopped;
		for (auto it = _playlist.begin(); it != _playlist.end(); ++it) {
			it->second->stop();
		}
		_currentPlaying = _playlist.end();
		_playlist.clear();
	}

	void MusicMenager::setPlayingMode(PlayingMode mode) {
		_playingMode = mode;
	}

	void MusicMenager::pause() {
		if(_status == Status::Playing) {
			_currentPlaying->second->pause();
			_status = Status::Paused;
		}
	}

	void MusicMenager::setVolume(float volume, const std::string& fileName) {
		if(fileName != "") 
			_playlist[fileName]->setVolume(volume);
		else {
			for (auto it = _playlist.begin(); it != _playlist.end(); ++it) {
				it->second->setVolume(volume);
			}
			_globalVolume = volume;
		}
	}

	inline std::string MusicMenager::getPath(const std::string& fileName) const {
		return "res/Music/" + fileName + ".flac"; 
	}

	bool MusicMenager::loadMusicByName(const std::string& fileName) {
		_playlist[fileName] = new sf::Music;
		if(!_playlist[fileName]->openFromFile(getPath(fileName)))
			return false;
		_playlist[fileName]->setVolume(_globalVolume);
		return true;
	}

	inline bool MusicMenager::isInPlaylist(const std::string& fileName) const {
		return _playlist.count(fileName);
	}

	std::string MusicMenager::getNameRandomMusicFile() const {
		auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		std::mt19937 mt(seed);
		std::uniform_int_distribution<int> random(0, _playlist.size() - 1);
		
		Playlist::const_iterator it;
		do {
			it = _playlist.begin();
			std::advance(it, random(mt));
		} while (it == _currentPlaying);

		return it->first;
	}

	void MusicMenager::playNextMusicFile() {
		if (_playingMode == PlayingMode::Orderly) {
			if (std::next(_currentPlaying) != _playlist.end())
				++_currentPlaying;
			else
				_currentPlaying = _playlist.begin();
			play(_currentPlaying->first);
		}
		if (_playingMode == PlayingMode::Random)
			play(getNameRandomMusicFile());
	}
}
