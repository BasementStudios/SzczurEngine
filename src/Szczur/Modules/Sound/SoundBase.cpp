#include "SoundBase.hpp"

#include <nlohmann/json.hpp>

namespace rat
{
    SoundBase::SoundBase()
    {

    }

    SoundBase::SoundBase(const std::string& name)
        : _name(name)
    {

    }

    void SoundBase::init()
    {
        _length = _buffer->getDuration().asSeconds();

        offset.beginTime = 0;
        offset.endTime   = _length;
    }

    void SoundBase::initScript(Script& script)
    {
        auto object = script.newClass<SoundBase>("SoundBase", "Sound");

        object.set("play", &SoundBase::play);
		object.set("stop", &SoundBase::stop);
		object.set("pause", &SoundBase::pause);
		object.set("getVolume", &SoundBase::getVolume);
		object.set("setVolume", &SoundBase::setVolume);
		object.set("getPitch", &SoundBase::getPitch);
		object.set("setPitch", &SoundBase::setPitch);
		object.set("isRelativeToListener", &SoundBase::isRelativeToListener);
		object.set("setRelativeToListener", &SoundBase::setRelativeToListener);
		object.set("getAttenuation", &SoundBase::getAttenuation);
		object.set("setAttenuation", &SoundBase::setAttenuation);
        object.set("getMinDistance", &SoundBase::getMinDistance);
		object.set("setMinDistance", &SoundBase::setMinDistance);
		object.set("setPosition", &SoundBase::setPosition);
        object.set("getPosition", &SoundBase::getPosition);
        object.set("getLoop", &SoundBase::getLoop);
        object.set("setLoop", &SoundBase::setLoop);
        object.set("setOffset", &SoundBase::setOffset);
        object.set("getLength", &SoundBase::getLength);
        object.set("getBeginTime", &SoundBase::getBeginTime);
        object.set("getEndTime", &SoundBase::getEndTime);

        object.set("getEqualizer", &SoundBase::getEffect<Equalizer>);
		object.set("getReverb", &SoundBase::getEffect<Reverb>);
		object.set("getEcho", &SoundBase::getEffect<Echo>);

		object.set("cleanEqualizer", &SoundBase::cleanEffect<Equalizer>);
		object.set("cleanReverb", &SoundBase::cleanEffect<Reverb>);
		object.set("cleanEcho", &SoundBase::cleanEffect<Echo>);

		object.init();
    }

    void SoundBase::setBuffer(sf::SoundBuffer* buffer)
    {
        _buffer = buffer;
        
        if(_buffer != nullptr)
            _sound.setBuffer(*_buffer);
    }

    void SoundBase::setVolume(float volume)
    {
        _volume = volume;
        _sound.setVolume(volume * (SoundVolume / 100));
    }

    float SoundBase::getVolume() const
    {
        return _volume;
    }

    float SoundBase::GetSoundVolume()
    {
        return SoundVolume;
    }

    void SoundBase::SetSoundVolume(float volume)
    {
        SoundVolume = volume;
    }

    void SoundBase::setPitch(float pitch)
    {
        _pitch = pitch;
        _sound.setPitch(pitch);
    }

    float SoundBase::getPitch() const
    {
        return _sound.getPitch();
    }

    bool SoundBase::isRelativeToListener() const
    {
        return _sound.isRelativeToListener();
    } 

    void SoundBase::setRelativeToListener(bool relative)
    {
        _sound.setRelativeToListener(relative);
    }

    float SoundBase::getAttenuation() const
    {
        return _sound.getAttenuation();
    }

    void SoundBase::setAttenuation(float attenuation) 
    {
        _sound.setAttenuation(attenuation);
    }

    float SoundBase::getMinDistance() const
    {
        return _sound.getMinDistance();
    }

    void SoundBase::setPosition(float x, float y, float z) 
    {
        _sound.setPosition(x, y, z);
    }

    sf::Vector3f SoundBase::getPosition() const
    {
        return _sound.getPosition();
    }

    void SoundBase::setMinDistance(float minDistance) 
    {
        _sound.setMinDistance(minDistance);
    }

    void SoundBase::setLoop(bool loop)
    {
        _sound.setLoop(loop);
    }

    bool SoundBase::getLoop() const
    {
        return _sound.getLoop();
    }

    void SoundBase::play()
    {
        _sound.play();

        if (_playingTime > offset.beginTime && _playingTime < offset.endTime)
            _sound.setPlayingOffset(sf::seconds(_playingTime));
        else
            _sound.setPlayingOffset(sf::seconds(offset.beginTime)); 
    }

    void SoundBase::pause()
    {
        _sound.pause();
        _playingTime = _sound.getPlayingOffset().asSeconds();
    }

    void SoundBase::stop()
    {
        _playingTime = 0;
        _sound.stop();
    }

    void SoundBase::setName(const std::string& name)
    {
        _name = name;
    }

    const std::string SoundBase::getName() const
    {
        return _name;
    }

    std::string SoundBase::getFileName() const
    {
        return _fileName;
    }

    void SoundBase::setOffset(Second_t beginT, Second_t endT)
    {
        if ((beginT >= _length || beginT < 0) && (endT > _length || endT < 0)) {
            offset.beginTime = 0;
            offset.endTime   = _length;
        }
        else if (beginT >= _length || beginT < 0) {
            offset.beginTime = 0;
            offset.endTime   = endT;
        }
        else if (endT > _length || endT < 0 || endT < beginT) {
            offset.beginTime = beginT;
            offset.endTime   = _length;
        }
        else {
            offset.beginTime = beginT;
            offset.endTime   = endT;
        }

        if (offset.endTime < offset.beginTime)
            offset.endTime = _length;    
    }

    SoundBase::Second_t SoundBase::getLength() const
    {
        return _length;
    }

    SoundBase::Second_t SoundBase::getBeginTime() const
    {
        return offset.beginTime;
    }

    SoundBase::Second_t SoundBase::getEndTime() const
    {
        return offset.endTime;
    }

    void SoundBase::load() 
    {
        nlohmann::json j;
        std::ifstream file(SOUND_DATA_FILE_PATH);    

        if (file.is_open()) {
            file >> j;
            file.close();

            if(j[_name]["Path"] == nullptr) {
                LOG_INFO("Missing data of sound: ", _name);
                return;
            }

            _fileName = j[_name]["Path"].get<std::string>();

            setVolume(j[_name]["Volume"]);
            setPitch(j[_name]["Pitch"]);
            setOffset(j[_name]["BeginTime"], j[_name]["EndTime"]);
            setAttenuation(j[_name]["Attenuation"]);
            setMinDistance(j[_name]["MinDistance"]);
            setRelativeToListener(j[_name]["Relative"]);
        }
    }
}
