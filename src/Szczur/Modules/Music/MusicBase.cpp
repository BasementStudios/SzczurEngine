#include "MusicBase.hpp"

#include <string>

#include <SFML/System.hpp>

namespace rat
{ 

	MusicBase::MusicBase(RatMusic* source)
		: _base(source)
	{
		_timeLeft = getDuration();
		setVolume(_base->getVolume());
	}

	void MusicBase::initScript(Script& script)
    {
        auto object = script.newClass<MusicBase>("MusicBase", "Music");

        object.set("play", &MusicBase::play);
		object.set("stop", &MusicBase::stop);
		object.set("pause", &MusicBase::pause);

		object.set("getVolume", &MusicBase::getVolume);
		object.set("setVolume", &MusicBase::setVolume);
		object.set("getLoop", &MusicBase::getLoop);
		object.set("setLoop", &MusicBase::setLoop);
		object.set("getStatus", &MusicBase::getStatus);
		object.set("getFadeTime", &MusicBase::getFadeTime);
		object.set("getDuration", &MusicBase::getDuration);
		object.set("getTimeLeft", &MusicBase::getTimeLeft);
		object.set("getName", &MusicBase::getName);

        object.set("getEqualizer", &MusicBase::getEffect<Equalizer>);
		object.set("getReverb", &MusicBase::getEffect<Reverb>);
		object.set("getEcho", &MusicBase::getEffect<Echo>);
		object.set("cleanEqualizer", &MusicBase::cleanEffect<Equalizer>);
		object.set("cleanReverb", &MusicBase::cleanEffect<Reverb>);
		object.set("cleanEcho", &MusicBase::cleanEffect<Echo>);

		object.setProperty("onStart", 
            [](){}, 
            [](MusicBase& obj, MusicBase::SolFunction_t callback){ obj.setCallback(MusicBase::CallbackType::onStart, callback); }
        );
		object.setProperty("onFinish", 
            [](){}, 
            [](MusicBase& obj, MusicBase::SolFunction_t callback){ obj.setCallback(MusicBase::CallbackType::onFinish, callback); }
        );
        object.setProperty("onFadeStart", 
            [](){}, 
            [](MusicBase& obj, MusicBase::SolFunction_t callback){ obj.setCallback(MusicBase::CallbackType::onFadeStart, callback); }
        );

		object.init();
    }

	void MusicBase::update(float deltaTime) 
	{
		if (getStatus() == sf::SoundSource::Status::Playing && !_finishing) {
			_timeLeft -= deltaTime;
			if (_timeLeft <= _base->getFadeTime()) {
				callback(CallbackType::onFadeStart);
				_isEnding = true;
			}
		}

		if (_timeLeft >= 0) {
			stop();
		}
	}

	bool MusicBase::isEnding() 
	{
		return _isEnding;
	}

	bool MusicBase::finish(float deltaTime) 
	{

		if (_finishInit) { 	
			_finishInit = false;
			_timeLeft = _base->getFadeTime();
			_finishing = true;
			_isEnding = false;
		}

		_timeLeft -= deltaTime;

		if (_timeLeft >= 0) {
			_base->setVolume((_timeLeft / _base->getFadeTime()) * _baseVolume);
			return false;
		}
		
		stop();
		return true;
	}

	void MusicBase::start(float deltaTime, float introTime)
	{
		if (_startInit) {
			_timeLeft = getDuration();
			_base->setVolume(0);
			_startInit = false;
		}
		
		update(deltaTime);

		auto vol = (_baseVolume * (getDuration() - _timeLeft)) / introTime;
		if (vol > 100) vol = 100;
		_base->setVolume(vol);
	}

	void MusicBase::play() 
	{
		callback(CallbackType::onStart);
		_base->play();
	}

	void MusicBase::pause() 
	{
		_base->pause();
	}

	void MusicBase::stop() 
	{
		callback(CallbackType::onFinish);
		_base->stop();
		reset();
	}

	void MusicBase::setCallback(MusicBase::CallbackType type, MusicBase::SolFunction_t callback)
    {
        _callbacks.insert_or_assign(type, callback);
    }

	void MusicBase::setLoop(bool loop)
	{
		_base->setLoop(loop);
	}

	bool MusicBase::getLoop() const
	{
		return _base->getLoop();
	}

	sf::SoundSource::Status MusicBase::getStatus() const 
	{
		return _base->getStatus();
	}

	float MusicBase::getFadeTime() const 
	{
		return _base->getFadeTime();
	}

	float MusicBase::getDuration() const 
	{
		return _base->getDuration().asSeconds();
	}

	float MusicBase::getTimeLeft() const
	{
		return _timeLeft;
	}

	void MusicBase::setTimeLeft(float timeLeft)
	{
		_timeLeft = timeLeft;
	}

	float MusicBase::getVolume() const 
	{
		return _baseVolume;
	}

	void MusicBase::setVolume(float volume) 
	{
		if (volume > 100) volume = 100;
		else if (volume < 0) volume = 0;

		_baseVolume = volume;
		_base->setVolume(_baseVolume);
	}
	
	void MusicBase::reset() 
	{
		_timeLeft = getDuration();
		_finishing = false;
		_finishInit = true;
		_startInit = true;
	}

	const std::string& MusicBase::getName() const
	{
		return _base->getName();
	}

	RatMusic* MusicBase::getSource() const
	{
		return _base;
	}

	void MusicBase::callback(MusicBase::CallbackType type)
    {
        if (auto it = _callbacks.find(type); it != _callbacks.end())
            std::invoke(it->second, this);
    }

}