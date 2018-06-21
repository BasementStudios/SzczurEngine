#include "SoundBase.hpp"

namespace rat
{
    bool SoundBase::init(const std::string& fileName)
    {
        _fileName = fileName;

        if (!loadBuffer())
            return false;

        _length = buffer.getDuration().asSeconds();

        offset.beginTime = 0;
        offset.endTime   = _length;

        return true;
    };

    void SoundBase::setVolume(float volume)
    {
        sound.setVolume(volume);
    }

    void SoundBase::setBaseVolume(float volume)
    {
        _volume = volume;
        sound.setVolume(volume);
    }

    float SoundBase::getVolume() const
    {
        return _volume;
    }

    void SoundBase::setPitch(float pitch)
    {
        _pitch = pitch;
        sound.setPitch(pitch);
    }

    float SoundBase::getPitch() const
    {
        return sound.getPitch();
    }

    void SoundBase::setLoop(bool loop)
    {
        sound.setLoop(loop);
    }

    bool SoundBase::getLoop() const
    {
        return sound.getLoop();
    }

    void SoundBase::play()
    {
        sound.setBuffer(buffer);
        sound.play();

        if (playingTime > offset.beginTime && playingTime < offset.endTime)
            sound.setPlayingOffset(sf::seconds(playingTime));
        else
            sound.setPlayingOffset(sf::seconds(offset.beginTime)); 
    }

    void SoundBase::pause()
    {
        sound.pause();
        playingTime = sound.getPlayingOffset().asSeconds();
    }

    void SoundBase::stop()
    {
        playingTime = 0;
        sound.stop();
    }

    const std::string SoundBase::getName() const
    {
        return _fileName;
    }

    bool SoundBase::loadBuffer()
    {
        return buffer.loadFromFile(getPath());
    }

    void SoundBase::setOffset(Second_t beginT,Second_t endT)
    {
        if (beginT >= _length || beginT < 0 && endT > _length || endT < 0) {
            offset.beginTime = 0;
            offset.endTime   = _length;
        }
        else if (beginT >= _length || beginT < 0) {
            offset.beginTime = 0;
            offset.endTime   = endT;
        }
        else if (endT > _length || endT<0 || endT < beginT) {
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

    std::string SoundBase::getPath() const
    {
        return "Assets/Sounds/" + _fileName + ".flac";
    }
}
