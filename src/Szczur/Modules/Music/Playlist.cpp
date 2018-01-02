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
			if (_playlist[_currentID]->isEnding()) {
				_endingID = _currentID;
				_isFileEnding = true;
				_status = Status::Stopped;
				playNext();
			}
			if (_isFileEnding) {
				_playlist[_currentID]->start(deltaTime, _playlist[_endingID]->getPostTime());
				if (_playlist[_endingID]->finish(deltaTime))
					_isFileEnding = false;
			}
			else
				_playlist[_currentID]->update(deltaTime);
		}
	}

	bool Playlist::add(const std::string& fileName)
	{		
		return (includes(fileName) || loadMusic(fileName));
	}

	void Playlist::remove(const std::string& fileName) 
	{
		if (_playlist[_currentID]->getName() == fileName) {
			_status = Status::Stopped;
			_playlist[_currentID]->stop();
		}
		_playlist.erase(_playlist.begin() + getID(fileName));
	}

	void Playlist::play(unsigned int id)
	{
		if(id >= _playlist.size()) {
			_currentID = _playlist.size();
			return;
		}

		if (!hasBeenEverPlayed)
			hasBeenEverPlayed = true;
			
		if (_status == Status::Playing)
			_playlist[_currentID]->stop();

		_currentID = id;
		std::cout << _playlist[_currentID]->getName() << std::endl; //For Tests
		_status = Status::Playing;
		_playlist[_currentID]->play();
	}

	void Playlist::play(const std::string& fileName) 
	{
		unsigned int newId = 0;
		if (fileName == "") {
			if (_status == Status::Paused) {
				unPause();
				return;
			}
			if (_playingMode == PlayingMode::Random)
				newId = getRandomId();
		}
		else
			newId = getID(fileName);

		play(newId);
	}

	void Playlist::clear() 
	{
		_status = Status::Stopped;
		for (auto it : _playlist)
			it->stop();
		_currentID = 0;
		_playlist.clear();
	}

	void Playlist::setPlayingMode(PlayingMode mode) 
	{
		_playingMode = mode;
	}

	void Playlist::pause() 
	{
		if (_status == Status::Playing) {
			_playlist[_currentID]->pause();
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
		return _playlist[_playlist.size() - 1]->init(fileName, _globalVolume);
	}

	inline bool Playlist::includes(const std::string& fileName) const 
	{
		return (getID(fileName) != _playlist.size());
	}

	unsigned int Playlist::getRandomId() const 
	{
		unsigned int nextID = 0;

		if (_playlist.size() > 1) {
			auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
			std::mt19937 mt(seed);
			std::uniform_int_distribution<int> random(0, _playlist.size() - 1);
			
			do {
				nextID = random(mt);
			} while (nextID == _currentID);
		}

		return nextID;
	}

	bool Playlist::setNewPlaylist(std::vector<std::string> newPlaylist) 
	{	
		clear();
		_currentID = newPlaylist.size();
		for (auto it : newPlaylist) {
			if (!add(it))
				return false;
		}
		return true;
	}

	void Playlist::playNext() 
	{
		if (_playingMode == PlayingMode::Orderly) {
			if (_currentID != _playlist.size() - 1)
				++_currentID;
			else
				_currentID = 0;
			play(_currentID);
		}
		if (_playingMode == PlayingMode::Random)
			play(getRandomId());
	}

	void Playlist::unPause() 
	{
		if (_status == Status::Paused) {
			_playlist[_currentID]->play();
			_status = Status::Playing;
		}
	}

	unsigned int Playlist::getID(const std::string& fileName) const
	{
		for (unsigned int i = 0; i < _playlist.size(); ++i) {
			if (_playlist[i]->getName() == fileName)
				return i;
		}
		return _playlist.size();
	}

}