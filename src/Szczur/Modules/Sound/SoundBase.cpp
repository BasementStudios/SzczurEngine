#include "SoundBase.hpp"

#include <nlohmann/json.hpp>

namespace rat
{
    void SoundBase::init()
    {
        _length = _buffer.getDuration().asSeconds();

        offset.beginTime = 0;
        offset.endTime   = _length;
    }

    void SoundBase::setBuffer(sf::SoundBuffer& buffer)
    {
        _buffer = buffer;
    }

    void SoundBase::setVolume(float volume)
    {
        _volume = volume;
        _sound.setVolume(volume);
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

    void SoundBase::load(const std::string& fileName) 
    {
        nlohmann::json j;
        std::ifstream file(fileName);    

        if (file.is_open()) {
            file >> j;
            file.close();

            _fileName = static_cast<std::string>(j["FileName"]);
            _name = static_cast<std::string>(j["Name"]);

            setVolume(j["Volume"]);
            setPitch(j["Pitch"]);
            setOffset(j["BeginTime"], j["EndTime"]);
            setAttenuation(j["Attenuation"]);
            setMinDistance(j["MinDistance"]);
            setRelativeToListener(j["Relative"]);
        }
    }
}
