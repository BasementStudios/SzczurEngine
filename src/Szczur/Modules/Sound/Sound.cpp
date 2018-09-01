#include "Sound.hpp"

namespace rat
{

    Sound::Sound()
    {
        initScript();
		LOG_INFO(this, " : Module Sound constructed");
    }

    Sound::~Sound()
    {
        for (auto& it : _sounds) {
            delete it;
        }

        LOG_INFO(this, " : Module Sound destructed");
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
        module.set_function("addNewSound", &Sound::addNewSound, this);
        module.set_function("removeSound", &Sound::removeSound, this);

        module.set("Stopped", SoundBase::Status::Stopped);
		module.set("Paused", SoundBase::Status::Paused);
		module.set("Playing", SoundBase::Status::Playing);


        script.initClasses<SoundBase>();
    }

    void Sound::update()
    {
        for (auto& sound : _sounds) {
            if (sound->getStatus() == Status::Playing)
                sound->update();
        }
    }

    SoundBase* Sound::addNewSound(const std::string& name)
    {
        auto sound = new SoundBase(getAssetsManager(), name);

        if (!sound->load()) {
            sound->setFileName(name);
        }

        auto fileName = sound->getFileName();
        if (fileName.empty()) {
            delete sound;
            return nullptr;
        }
        
        addSound(sound);

        if (_soundAssets.load(fileName) && 
            sound->setBuffer(_soundAssets.get(fileName)) 
        ){
            sound->init();
            LOG_INFO("[Sound] Added: ", name);
            return sound;
        }
            
        delete sound;
        return nullptr;
    }

    void Sound::addSound(SoundBase* sound)
    {
        _sounds.emplace_back(sound);
    }

    void Sound::removeSound(SoundBase* sound)
    {
       _sounds.remove_if([=](auto it){ 
            bool same = (it == sound);
            if (same) {
                LOG_INFO("[Sound] Removed: ", sound->getName());
                delete sound;
            }
            return same;
        });
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
