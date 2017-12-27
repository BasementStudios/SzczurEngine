#include "Playlist.hpp"

#include <random>
#include <chrono>
#include <algorithm>

#include <iostream>

namespace rat 
{

	Playlist::Playlist(std::vector<std::string> newPlaylist) 
	{
		setNewPlaylist(newPlaylist);
	}

	void Playlist::update(float deltaTime) 
	{
		if(hasBeenEverPlayed) {
			if (_currentPlaying->second->isEnding()) {
				_endingFile = _currentPlaying;
				_isFileEnding = true;
				playNext();
			}
			if (_isFileEnding) {
				_currentPlaying->second->start(deltaTime, _endingFile->second->getPostTime());
				if (_endingFile->second->finish(deltaTime))
					_isFileEnding = false;
			}
			else
				_currentPlaying->second->update(deltaTime);
		}
	}

	bool Playlist::add(const std::string& fileName)
	{
		return (includes(fileName) || loadMusic(fileName));
	}

	void Playlist::remove(const std::string& fileName) 
	{
		if (_currentPlaying->first == fileName) {
			_status = Status::Stopped;
			_currentPlaying->second->stop();
		}
		_playlist.erase(fileName);
	}

	void Playlist::play(const std::string& fileName) 
	{
		if (!hasBeenEverPlayed)
			hasBeenEverPlayed = true;
		if (_status == Status::Playing)
			_currentPlaying->second->stop();
		if (fileName == "") {
			if (_status == Status::Paused) {
				unPause();
				return;
			}
			if (_playingMode == PlayingMode::Random)
				_currentPlaying = _playlist.find(getNameOfRandomFile());
			else
				_currentPlaying = _playlist.begin();
		}
		else if (includes(fileName))
			_currentPlaying = _playlist.find(fileName);

		std::cout << _currentPlaying->first << std::endl; //For Tests
		_status = Status::Playing;
		_currentPlaying->second->play();
	}

	void Playlist::clear() 
	{
		_status = Status::Stopped;
		for (auto it = _playlist.begin(); it != _playlist.end(); ++it) {
			it->second->stop();
		}
		_currentPlaying = _playlist.end();
		_playlist.clear();
	}

	void Playlist::setPlayingMode(PlayingMode mode) 
	{
		_playingMode = mode;
	}

	void Playlist::pause() 
	{
		if (_status == Status::Playing) {
			_currentPlaying->second->pause();
			_status = Status::Paused;
		}
	}

	void Playlist::setVolume(float volume, const std::string& fileName) 
	{
		if(fileName != "")
			_playlist[fileName]->setVolume(volume * (_globalVolume / 100));
		else {
			_globalVolume = volume;
			for (auto it = _playlist.begin(); it != _playlist.end(); ++it)
				it->second->setVolume(it->second->getVolume() * (volume / 100));
		}
	}

	bool Playlist::loadMusic(const std::string& fileName) 
	{
		_playlist[fileName] =  std::make_shared<Music>();
		return _playlist[fileName]->init(fileName, 0, _globalVolume);
	}

	inline bool Playlist::includes(const std::string& fileName) const 
	{
		return _playlist.count(fileName);
	}

	std::string Playlist::getNameOfRandomFile() const 
	{
		auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		std::mt19937 mt(seed);
		std::uniform_int_distribution<int> random(0, _playlist.size() - 1);
		
		Container_t::const_iterator it;
		do {
			it = _playlist.begin();
			std::advance(it, random(mt));
		} while (it == _currentPlaying);

		return it->first;
	}

    bool Playlist::setNewPlaylist(std::vector<std::string> newPlaylist) 
	{	
		clear();
		std::reverse(newPlaylist.begin() , newPlaylist.end());
		for (auto it : newPlaylist) {
			if (!add(it))
				return false;
		}
    	return true;
    }

	void Playlist::playNext() 
	{
		if (_playingMode == PlayingMode::Orderly) {
			if (std::next(_currentPlaying) != _playlist.end())
				++_currentPlaying;
			else
				_currentPlaying = _playlist.begin();
			play(_currentPlaying->first);
		}
		if (_playingMode == PlayingMode::Random)
			play(getNameOfRandomFile());
	}

	void Playlist::unPause() 
	{
		if (_status == Status::Paused) {
			_currentPlaying->second->play();
			_status = Status::Playing;
		}
	}

}