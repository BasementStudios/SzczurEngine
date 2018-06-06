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
        module.set_function("getSound", &SoundManager::getSound, this);
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
    }

    bool SoundManager::newSound(const std::string &name,const std::string &fileName)
    {
        auto sound = std::make_unique<SoundBase>();
         
        if(sound->init(name, fileName)){
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

    void SoundManager::eraseSingleSound(const std::string &fileName)
    {
        int i = getSound(fileName);
        _sounds.erase(_sounds.begin() + i);
    }

    int SoundManager::getSound(const std::string &fileName) const
    {
        for(unsigned int i = 0; i < _sounds.size(); ++i){
            if(fileName == _sounds[i]->getName())
                return i;
        }

        return _sounds.size();
    }

    void SoundManager::setVolume(float volume, const std::string &fileName)
    {
        if(fileName==""){
            _volumeGame=volume;
            for(unsigned int i=0; i<_sounds.size(); ++i)
               _sounds[i]->setVolume(_sounds[i]->getVolume()*(volume/100));
        }
        else{
            int i = getSound(fileName);
            _sounds[i]->setBaseVolume(volume*(_volumeGame/100));
        }
    }

    void SoundManager::setPitch(float pitch, const std::string &fileName)
    {
        int i = getSound(fileName);
        _sounds[i]->setPitch(pitch);
    }
 
    void SoundManager::setLoop(bool loop,const std::string &fileName)
    {
        int i = getSound(fileName);
        _sounds[i]->setLoop(loop);
    }

    void SoundManager::changeLoop(const std::string &fileName)
    {
        if(fileName.empty()) {
            for(unsigned int i=0; i<_sounds.size(); ++i){
                if(_sounds[i]->getLoop())
                    _sounds[i]->setLoop(false);
                else
                    _sounds[i]->setLoop(true);
            }
            return;
        }

        int i = getSound(fileName);
        if(_sounds[i]->getLoop())
            _sounds[i]->setLoop(false);
        else
            _sounds[i]->setLoop(true);
    }

    bool SoundManager::getLoop(const std::string &fileName)
    {
        int i = getSound(fileName);
        return _sounds[i]->getLoop();
    }

    int SoundManager::getSize() const
    {
        return _sounds.size();
    }

    void SoundManager::play(const std::string &fileName)
    {
        if (fileName.empty()) {
            for (unsigned int i=0; i<_sounds.size(); ++i)
                _sounds[i]->play();
            return;
        }

        int i = getSound(fileName);
        _sounds[i]->play();
    }

    void SoundManager::pause(const std::string &fileName)
    {
        if (fileName.empty()) {
            for (unsigned int i=0; i<_sounds.size(); ++i)
                _sounds[i]->pause();
            return;
        }

        int i = getSound(fileName);
        _sounds[i]->pause();
    }

    void SoundManager::stop(const std::string &fileName)
    {
        if (fileName.empty()) {
            for (unsigned int i = 0; i < _sounds.size(); ++i) {
                _sounds[i]->setLoop(false);
                _sounds[i]->stop();
            }
            return;
        }

        int i = getSound(fileName);
        _sounds[i]->setLoop(false);
        _sounds[i]->stop();
    }

    void SoundManager::setOffset(const std::string &fileName, Second_t beginT, Second_t endT)
    {
        int i = getSound(fileName);
        _sounds[i]->setOffset(beginT,endT);
    }

    SoundManager::Second_t SoundManager::getLength(const std::string &fileName) const
    {
        int i = getSound(fileName);
        return _sounds[i]->getLength();
    }

    std::string SoundManager::getName(int i) const
    {
        return _sounds[i]->getName();
    }

    void SoundManager::load(const std::string& fileName)
    {
        nlohmann::json j;
        std::ifstream file("Assets/Sounds/Data/" + fileName + ".json");    

        if (file.is_open()) {
            file >> j;
            file.close();

            auto name = j["Name"];

            newSound(name, j["File"]);
            setVolume(j["Volume"], name);
            setPitch(j["Pitch"], name);
            setOffset(name, j["BeginTime"], j["EndTime"]);

        }
    }

}
