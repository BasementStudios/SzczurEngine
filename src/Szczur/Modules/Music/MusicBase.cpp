#include "MusicBase.hpp"

#include <Szczur/Json.hpp>

#include <fstream>
#include <cmath>

namespace rat
{ 

	bool MusicBase::init(const std::string& fileName, float volume)
	{
		_name = fileName;
		if (!loadMusic())
			return false;
		_timeLeft = _base.getDuration().asSeconds();
		getJsonData();
		setVolume(volume);
		_postTime = std::fmod(_timeLeft, (240 / _bpm)); 
		return true;
	};

	void MusicBase::update(float deltaTime) 
	{
		if (getStatus() != sf::SoundSource::Status::Paused) {
			_timeLeft -= deltaTime;
			if (_timeLeft <= _postTime)
				_isEnding = true;
		}
	}

	bool MusicBase::isEnding() 
	{
		return _isEnding;
	}

	bool MusicBase::finish(float deltaTime) 
	{
		_isEnding = false;
		_timeLeft -= deltaTime;
		if (_timeLeft > 0) {
			float volume = (_timeLeft / _postTime) * _baseVolume;
			_base.setVolume(volume);
			return false;
		}
		reset();
		stop();
		return true;
	}

	void MusicBase::start(float deltaTime, float preTime) 
	{
		update(deltaTime);
		float volume = (_baseVolume * (getDuration() - _timeLeft)) / preTime;
		_base.setVolume(volume);
	}

	bool MusicBase::loadMusic() 
	{
		return _base.openFromFile(getPath());
	}

	void MusicBase::getJsonData() 
	{
		Json json;
		std::ifstream file("res/Music/Music.json");
		if(file.is_open()){
            file >> json;
			file.close();
		}
		_bpm = json[_name]["BPM"];
	}

	void MusicBase::play() 
	{
		_base.play();
	}

	void MusicBase::pause() 
	{
		_base.pause();
	}

	void MusicBase::stop() 
	{
		_base.stop();
	}

	void MusicBase::setLoop(bool loop)
	{
		_base.setLoop(loop);
	}

	bool MusicBase::getLoop() const
	{
		return _base.getLoop();
	}

	sf::SoundSource::Status MusicBase::getStatus() const 
	{
		return _base.getStatus();
	}

	float MusicBase::getPostTime() const 
	{
		return _postTime;
	}

	float MusicBase::getDuration() const 
	{
		return _base.getDuration().asSeconds();
	}

	float MusicBase::getVolume() const 
	{
		return _baseVolume;
	}

	void MusicBase::setVolume(float volume) 
	{
		_baseVolume = volume;
		_base.setVolume(volume);
	}

	inline std::string MusicBase::getPath() const 
	{
		return "res/Music/" + _name + ".flac"; 
	}

	void MusicBase::reset() 
	{
		_timeLeft = _base.getDuration().asSeconds();
		_base.setVolume(_baseVolume);
	}

	const std::string& MusicBase::getName() const
	{
		return _name;
	}

}