#include "MusicBase.hpp"

#include <Szczur/Json.hpp>

#include <fstream>
#include <cmath>

namespace rat
{ 

	bool MusicBase::init(const std::string& fileName, float volume)
	{
		_name = fileName;
		if (loadMusic()) {
			_timeLeft = getDuration();
			getJsonData();
			setVolume(volume);
			return true;
		}
		return false;
	};

	void MusicBase::update(float deltaTime) 
	{
		if (getStatus() == sf::SoundSource::Status::Playing && !_finishing) {
			_timeLeft -= deltaTime;
			if (_timeLeft <= _fadeTime)
				_isEnding = true;
		}
	}

	bool MusicBase::isEnding() 
	{
		return _isEnding;
	}

	bool MusicBase::finish(float deltaTime) 
	{
		static float currVolume = _baseVolume;

		if (_finishInit) {
			currVolume = _base.getVolume();
			_timeLeft = _fadeTime;
			_finishing = true;
			_finishInit = false;
			_isEnding = false;
		}

		_timeLeft -= deltaTime;

		if (_timeLeft >= 0) {
			float volume = (_timeLeft / _fadeTime) * currVolume;
			_base.setVolume(volume);
			return false;
		}
		currVolume = _baseVolume;
		reset();
		stop();
		return true;
	}

	void MusicBase::start(float deltaTime, float introTime)
	{
		if (_startInit) {
			_timeLeft = getDuration();
			_base.setVolume(0);
			_startInit = false;
		}
		update(deltaTime);
		float volume = (_baseVolume * (getDuration() - _timeLeft)) / introTime;
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
		float numberOfBars = json[_name]["FadeTime"];

		if(numberOfBars > 0) {
			auto barTime = 240 / _bpm;
			_fadeTime = barTime * numberOfBars;
		}
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

	float MusicBase::getFadeTime() const 
	{
		return _fadeTime;
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
		_finishing = false;
		_finishInit = true;
		_startInit = true;
	}

	const std::string& MusicBase::getName() const
	{
		return _name;
	}

}