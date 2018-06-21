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
        module.set_function("setPitch", &SoundManager::setPitch, this);
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

    bool SoundManager::newSound(const std::string& fileName)
    {
        auto sound = std::make_unique<SoundBase>();
         
        if (sound->init(fileName)) {
            _sounds.push_back(std::move(sound));
            return true;
        }

        _sounds.back()->setVolume(_sounds.back()->getVolume() * (_volumeGame / 100));

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

    void SoundManager::eraseSingleSound(const std::string& fileName)
    {
        int id = getSoundID(fileName);
        _sounds.erase(_sounds.begin() + id);
    }

    int SoundManager::getSoundID(const std::string& fileName) const
    {
        for(unsigned int i = 0; i < _sounds.size(); ++i){
            if(fileName == _sounds[i]->getName())
                return i;
        }

        return _sounds.size();
    }

    void SoundManager::setVolume(float volume, const std::string& fileName)
    {
        if (fileName == "") {
            _volumeGame = volume;
            for (unsigned int i = 0; i < _sounds.size(); ++i)
               _sounds[i]->setVolume(_sounds[i]->getVolume() * (volume / 100));
        }
        else {
            int id = getSoundID(fileName);
            _sounds[id]->setBaseVolume(volume * (_volumeGame / 100));
        }
    }

    void SoundManager::setPitch(float pitch, const std::string& fileName)
    {
        int id = getSoundID(fileName);
        _sounds[id]->setPitch(pitch);
    }
 
    void SoundManager::setLoop(bool loop, const std::string& fileName)
    {
        int id = getSoundID(fileName);
        _sounds[id]->setLoop(loop);
    }

    void SoundManager::changeLoop(const std::string& fileName)
    {
        if (fileName.empty()) {
            for (unsigned int i = 0; i < _sounds.size(); ++i) {
                if (_sounds[i]->getLoop())
                    _sounds[i]->setLoop(false);
                else
                    _sounds[i]->setLoop(true);
            }
            return;
        }

        int id = getSoundID(fileName);
        
        if(_sounds[id]->getLoop())
            _sounds[id]->setLoop(false);
        else
            _sounds[id]->setLoop(true);
    }

    bool SoundManager::getLoop(const std::string& fileName)
    {
        int id = getSoundID(fileName);
        return _sounds[id]->getLoop();
    }

    int SoundManager::getSize() const
    {
        return _sounds.size();
    }

    void SoundManager::play(const std::string& fileName)
    {
        if (fileName.empty()) {
            for (unsigned int i = 0; i < _sounds.size(); ++i)
                _sounds[i]->play();
            return;
        }

        int id = getSoundID(fileName);
        _sounds[id]->play();
    }

    void SoundManager::pause(const std::string& fileName)
    {
        if (fileName.empty()) {
            for (unsigned int i = 0; i < _sounds.size(); ++i)
                _sounds[i]->pause();
            return;
        }

        int id = getSoundID(fileName);
        _sounds[id]->pause();
    }

    void SoundManager::stop(const std::string& fileName)
    {
        if (fileName.empty()) {
            for (unsigned int i = 0; i < _sounds.size(); ++i) {
                _sounds[i]->setLoop(false);
                _sounds[i]->stop();
            }
            return;
        }

        int id = getSoundID(fileName);

        _sounds[id]->setLoop(false);
        _sounds[id]->stop();
    }

    void SoundManager::setOffset(const std::string& fileName, Second_t beginT, Second_t endT)
    {
        int id = getSoundID(fileName);
        _sounds[id]->setOffset(beginT, endT);
    }

    SoundManager::Second_t SoundManager::getLength(const std::string& fileName) const
    {
        int id = getSoundID(fileName);
        return _sounds[id]->getLength();
    }

    std::string SoundManager::getName(int id) const
    {
        return _sounds[id]->getName();
    }

    void SoundManager::load(const std::string& fileName)
    {
        nlohmann::json j;
        std::ifstream file("Assets/Sounds/Data/" + fileName + ".json");    

        if (file.is_open()) {
            file >> j;
            file.close();

            newSound(fileName);

            setVolume(j["Volume"], fileName);
            setPitch(j["Pitch"], fileName);
            setOffset(fileName, j["BeginTime"], j["EndTime"]);
        }
    }

}
