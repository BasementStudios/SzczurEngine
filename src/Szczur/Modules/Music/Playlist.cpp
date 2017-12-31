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
			if ((*_currentPlaying)->isEnding()) {
				_endingFile = _currentPlaying;
				_isFileEnding = true;
				_status = Status::Stopped;
				playNext();
			}
			if (_isFileEnding) {
				(*_currentPlaying)->start(deltaTime, (*_endingFile)->getPostTime());
				if ((*_endingFile)->finish(deltaTime))
					_isFileEnding = false;
			}
			else
				(*_currentPlaying)->update(deltaTime);
		}
	}

	bool Playlist::add(const std::string& fileName)
	{
		return (includes(fileName) || loadMusic(fileName));
	}

	void Playlist::remove(const std::string& fileName) 
	{
		if ((*_currentPlaying)->getName() == fileName) {
			_status = Status::Stopped;
			(*_currentPlaying)->stop();
		}
		_playlist.erase(_playlist.begin() + getID(fileName));
	}

	void Playlist::play(const std::string& fileName) 
	{
		if (!hasBeenEverPlayed)
			hasBeenEverPlayed = true;
			
		if (_status == Status::Playing)
			(*_currentPlaying)->stop();

		if (fileName == "") {
			if (_status == Status::Paused) {
				unPause();
				return;
			}
			if (_playingMode == PlayingMode::Random)
				_currentPlaying =  _playlist.begin() + getID(getNameOfRandomFile());
			else
				_currentPlaying = _playlist.begin();
		}
		else if (includes(fileName))
			_currentPlaying = _playlist.begin() + getID(fileName);

		std::cout << (*_currentPlaying)->getName() << std::endl; //For Tests
		_status = Status::Playing;
		(*_currentPlaying)->play();
	}

	void Playlist::clear() 
	{
		_status = Status::Stopped;
		for (auto it : _playlist)
			it->stop();
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
			(*_currentPlaying)->pause();
			_status = Status::Paused;
		}
	}

	void Playlist::setVolume(float volume, const std::string& fileName) 
	{
		if(fileName != "")
			_playlist[getID(fileName)]->setVolume(volume * (_globalVolume / 100));
		else {
			_globalVolume = volume;
			for (auto it : _playlist)
				it->setVolume(it->getVolume() * (volume / 100));
		}
	}

	bool Playlist::loadMusic(const std::string& fileName) 
	{
		_playlist.push_back(std::make_shared<MusicBase>());
		return (*(--_playlist.end()))->init(fileName, _globalVolume);
	}

	inline bool Playlist::includes(const std::string& fileName) const 
	{
		return (getID(fileName) != -1);
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

		return (*it)->getName();
	}

	bool Playlist::setNewPlaylist(std::vector<std::string> newPlaylist) 
	{	
		clear();
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
			play((*_currentPlaying)->getName());
		}
		if (_playingMode == PlayingMode::Random)
			play(getNameOfRandomFile());
	}

	void Playlist::unPause() 
	{
		if (_status == Status::Paused) {
			(*_currentPlaying)->play();
			_status = Status::Playing;
		}
	}

	int Playlist::getID(const std::string& fileName) const
	{
		for (unsigned int i = 0; i < _playlist.size(); ++i) {
			if (_playlist[i]->getName() == fileName)
				return i;
		}
		return -1;
	}

}