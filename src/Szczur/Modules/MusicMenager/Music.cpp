#include "Music.hpp"

#include <iostream>

namespace rat { 

	bool Music::init(const std::string& fileName, float postTime, float volume) {
		_postTime = postTime; 
		if(!loadMusic(fileName))
			return false;
		_timeLeft = _base.getDuration().asSeconds();
		setVolume(volume);
		return true;
	};

	void Music::update(float deltaTime) {
		if (getStatus() == sf::SoundSource::Status::Playing) {
			_timeLeft -= deltaTime;
			if (_timeLeft <= _postTime)
				_isEnding = true;
		}
	}

	bool Music::isEnding() {
		return _isEnding;
	}

	bool Music::finish(float deltaTime) {
		_isEnding = false;
		_timeLeft -= deltaTime;
		float volume = (_timeLeft / _postTime) * _baseVolume;
		if(_timeLeft > 0) {
			_base.setVolume(volume);
			return false;
		}
		reset();
		stop();
		return true;
	}

	void Music::start(float deltaTime, float preTime) {
		update(deltaTime);
		float volume = (_baseVolume * (getDuration() - _timeLeft)) / preTime;
		_base.setVolume(volume);
	}

	bool Music::loadMusic(const std::string& fileName) {
		return _base.openFromFile(getPath(fileName));
	}

	void Music::play() {
		_base.play();
	}

	void Music::pause() {
		_base.pause();
	}

	void Music::stop() {
		_base.stop();
	}

	sf::SoundSource::Status Music::getStatus() const {
		return _base.getStatus();
	}

	float Music::getPostTime() const {
		return _postTime;
	}

	float Music::getDuration() const {
		return _base.getDuration().asSeconds();
	}

	float Music::getVolume() const {
		return _baseVolume;
	}

	void Music::setVolume(float volume) {
		_baseVolume = volume;
		_base.setVolume(volume);
	}

	inline std::string Music::getPath(const std::string& fileName) const {
		return "res/Music/" + fileName + ".flac"; 
	}

	void Music::reset() {
		_timeLeft = _base.getDuration().asSeconds();
		_base.setVolume(_baseVolume);
	}

}