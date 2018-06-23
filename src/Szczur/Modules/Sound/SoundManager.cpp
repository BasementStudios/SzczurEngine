#include "SoundManager.hpp"

#include <Json/json.hpp>

namespace rat
{
    SoundManager::SoundManager()
    {
        initScript();
        LOG_INFO(this, " : Module SoundManager constructed");
    }

    SoundManager::~SoundManager()
    {
        LOG_INFO(this, " : Module SoundManager destructed");
    }

    void SoundManager::initScript()
    {
        Script& script = getModule<Script>();
		auto module = script.newModule("SoundManager");

        module.set_function("newSound", &SoundManager::newSound, this);
        module.set_function("eraseSounds", &SoundManager::eraseSounds, this);
        module.set_function("eraseSingleSound", &SoundManager::eraseSingleSound, this);
        module.set_function("getSoundID", &SoundManager::getSoundID, this);
        module.set_function("play", &SoundManager::play, this);
        module.set_function("pause", &SoundManager::pause, this);
        module.set_function("stop", &SoundManager::stop, this);
        module.set_function("setVolume", &SoundManager::setVolume, this);
        module.set_function("getVolume", &SoundManager::getVolume, this);
        module.set_function("isRelativeToListener", &SoundManager::isRelativeToListener, this);
        module.set_function("setRelativeToListener", &SoundManager::setRelativeToListener, this);
        module.set_function("getAttenuation", &SoundManager::getAttenuation, this);
        module.set_function("setAttenuation", &SoundManager::setAttenuation, this);
        module.set_function("getMinDistance", &SoundManager::getMinDistance, this);
        module.set_function("setMinDistance", &SoundManager::setMinDistance, this);
        module.set_function("setPitch", &SoundManager::setPitch, this);
        module.set_function("getPitch", &SoundManager::getPitch, this);
        module.set_function("setLoop", &SoundManager::setLoop, this);
        module.set_function("changeLoop", &SoundManager::changeLoop, this);
        module.set_function("getLoop", &SoundManager::getLoop, this);
        module.set_function("getSize", &SoundManager::getSize, this);
        module.set_function("setOffset", &SoundManager::setOffset, this);
        module.set_function("getLength", &SoundManager::getLength, this);
        module.set_function("getName", &SoundManager::getName, this);
        module.set_function("load", &SoundManager::load, this);

        module.set_function("getEqualizer", &SoundManager::getEffect<Equalizer>, this);
		module.set_function("getReverb", &SoundManager::getEffect<Reverb>, this);
		module.set_function("getEcho", &SoundManager::getEffect<Echo>, this);

		module.set_function("cleanEqualizer", &SoundManager::cleanEffect<Equalizer>, this);
		module.set_function("cleanReverb", &SoundManager::cleanEffect<Reverb>, this);
		module.set_function("cleanEcho", &SoundManager::cleanEffect<Echo>, this);
    }

    bool SoundManager::newSound(const std::string& fileName, const std::string& name)
    {
        auto sound = std::make_unique<SoundBase>();
         
        if (sound->init(fileName, name)) {
            _sounds.push_back(std::move(sound));
            return true;
        }

        return false;
    }

    void SoundManager::eraseSounds()
    {
        _sounds.clear();
    }

    void SoundManager::eraseSingleSoundByID(int id)
    {
        if (id >= 0 && id < _sounds.size())
            _sounds.erase(_sounds.begin() + id);
    }

    void SoundManager::eraseSingleSound(const std::string& name)
    {
        int id = getSoundID(name);
        _sounds.erase(_sounds.begin() + id);
    }

    int SoundManager::getSoundID(const std::string& name) const
    {
        for(unsigned int i = 0; i < _sounds.size(); ++i){
            if(name == _sounds[i]->getName())
                return i;
        }

        return -1;
    }

    void SoundManager::setVolume(float volume, const std::string& name)
    {
        if (name == "") {
            for (unsigned int i = 0; i < _sounds.size(); ++i)
               _sounds[i]->setVolume(_sounds[i]->getVolume() * (volume / 100));
        }
        else {
            int id = getSoundID(name);
            _sounds[id]->setVolume(volume);
        }
    }

    float SoundManager::getVolume(const std::string& name) const 
    {
        return _sounds[getSoundID(name)]->getVolume();
    }

    void SoundManager::setPitch(float pitch, const std::string& name)
    {
        if (name == "") {
            for (unsigned int i = 0; i < _sounds.size(); ++i)
               _sounds[i]->setPitch(pitch);
        }
        else {
            int id = getSoundID(name);
            _sounds[id]->setPitch(pitch);
        }
    }

    float SoundManager::getPitch(const std::string& name) const 
    {
        return _sounds[getSoundID(name)]->getPitch();
    }

    bool SoundManager::isRelativeToListener(const std::string& name) const 
    {
        return _sounds[getSoundID(name)]->isRelativeToListener();
    }

    void SoundManager::setRelativeToListener(bool relative, const std::string& name)
    {
        if (name == "") {
            for (unsigned int i = 0; i < _sounds.size(); ++i)
               _sounds[i]->setRelativeToListener(relative);
        }
        else {
            _sounds[getSoundID(name)]->setRelativeToListener(relative);
        }
    }

    float SoundManager::getAttenuation(const std::string& name) const 
    {
        return _sounds[getSoundID(name)]->getAttenuation();
    }

    void SoundManager::setAttenuation(float attenuation, const std::string& name)
    {
        if (name == "") {
            for (unsigned int i = 0; i < _sounds.size(); ++i)
               _sounds[i]->setAttenuation(attenuation);
        }
        else {
            _sounds[getSoundID(name)]->setAttenuation(attenuation);
        }
    }

    float SoundManager::getMinDistance(const std::string& name) const 
    {
        return _sounds[getSoundID(name)]->getMinDistance();
    }

    void SoundManager::setMinDistance(float minDistance, const std::string& name)
    {
        if (name == "") {
            for (unsigned int i = 0; i < _sounds.size(); ++i)
               _sounds[i]->setMinDistance(minDistance);
        }
        else {
            _sounds[getSoundID(name)]->setMinDistance(minDistance);
        }
    }

 
    void SoundManager::setLoop(bool loop, const std::string& name)
    {
        int id = getSoundID(name);
        _sounds[id]->setLoop(loop);
    }

    void SoundManager::changeLoop(const std::string& name)
    {
        if (name.empty()) {
            for (unsigned int i = 0; i < _sounds.size(); ++i) {
                if (_sounds[i]->getLoop())
                    _sounds[i]->setLoop(false);
                else
                    _sounds[i]->setLoop(true);
            }
            return;
        }

        int id = getSoundID(name);
        
        if(_sounds[id]->getLoop())
            _sounds[id]->setLoop(false);
        else
            _sounds[id]->setLoop(true);
    }

    bool SoundManager::getLoop(const std::string& name)
    {
        int id = getSoundID(name);
        return _sounds[id]->getLoop();
    }

    int SoundManager::getSize() const
    {
        return _sounds.size();
    }

    void SoundManager::play(const std::string& name)
    {
        if (name.empty()) {
            for (unsigned int i = 0; i < _sounds.size(); ++i)
                _sounds[i]->play();
            return;
        }

        int id = getSoundID(name);
        _sounds[id]->play();
    }

    void SoundManager::pause(const std::string& name)
    {
        if (name.empty()) {
            for (unsigned int i = 0; i < _sounds.size(); ++i)
                _sounds[i]->pause();
            return;
        }

        int id = getSoundID(name);
        _sounds[id]->pause();
    }

    void SoundManager::stop(const std::string& name)
    {
        if (name.empty()) {
            for (unsigned int i = 0; i < _sounds.size(); ++i) {
                _sounds[i]->setLoop(false);
                _sounds[i]->stop();
            }
            return;
        }

        int id = getSoundID(name);

        _sounds[id]->setLoop(false);
        _sounds[id]->stop();
    }

    void SoundManager::setOffset(const std::string& name, Second_t beginT, Second_t endT)
    {
        int id = getSoundID(name);
        _sounds[id]->setOffset(beginT, endT);
    }

    SoundManager::Second_t SoundManager::getLength(const std::string& name) const
    {
        int id = getSoundID(name);
        return _sounds[id]->getLength();
    }

    SoundManager::Second_t SoundManager::getBeginTime(const std::string& name) const
    {
        return _sounds[getSoundID(name)]->getBeginTime();
    }

    SoundManager::Second_t SoundManager::getEndTime(const std::string& name) const
    {
        return _sounds[getSoundID(name)]->getEndTime();
    }

    std::string SoundManager::getName(int id) const
    {
        return _sounds[id]->getName();
    }

    std::string SoundManager::getFileName(const std::string& name) const
    {
        return _sounds[getSoundID(name)]->getFileName();
    }

    void SoundManager::load(const std::string& fileName)
    {
        nlohmann::json j;
        std::ifstream file("Assets/Sounds/Data/" + fileName + ".json");    

        if (file.is_open()) {
            file >> j;
            file.close();

            std::string name = j["Name"];
            std::string path = j["Path"];

            newSound(path, name);
            
            setVolume(j["Volume"], name);
            setPitch(j["Pitch"], name);
            setOffset(name, j["BeginTime"], j["EndTime"]);
        }
    }

}
