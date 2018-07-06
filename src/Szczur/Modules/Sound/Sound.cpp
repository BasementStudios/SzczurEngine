#include "Sound.hpp"

namespace rat
{

    Sound::Sound()
    {
        initScript();
		LOG_INFO(this, " : Module Music constructed");
    }

    Sound::~Sound()
    {
        LOG_INFO(this, " : Module Music destructed");
    }

    void Sound::initScript()
    {
        Script& script = getModule<Script>();
		auto module = script.newModule("Sound");

        module.set_function("play", &Sound::play, this);
		module.set_function("pause", &Sound::pause, this);
		module.set_function("stop", &Sound::stop, this);
        module.set_function("setVolume", &Sound::setVolume, this);
		module.set_function("getVolume", &Sound::getVolume, this);

        script.initClasses<SoundBase>();
    }

    void Sound::addSound(SoundBase* sound)
    {
        _sounds.emplace_back(sound);
    }
    void Sound::removeSound(SoundBase* sound)
    {
       _sounds.remove_if([=](auto it){ return it == sound; });
    }

    void Sound::play()
    {
        for (auto& it : _sounds) {
            it->play();
        }
    }

	void Sound::pause()
    {
        for (auto& it : _sounds) {
            it->pause();
        }
    }

	void Sound::stop()
    {
        for (auto& it : _sounds) {
            it->stop();
        }
    }

	void Sound::setVolume(float volume)
    {
        SoundBase::SetSoundVolume(volume);

        for (auto& it : _sounds) {
            it->setVolume(it->getVolume());
        }
    }

	float Sound::getVolume() const
    {
        return SoundBase::GetSoundVolume();
    }

    SoundAssets& Sound::getAssetsManager()
    {
        return _soundAssets;
    }

}
