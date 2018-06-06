#include "SoundManager.hpp"

namespace rat
{
    bool SoundManager::newSound(const std::string &name,const std::string &fileName)
    {
        auto sound = std::make_unique<SoundBase>();
         
        if(sound->init(name,fileName)){
            _sounds.push_back(std::move(sound));
            return true;
        }
        return false;
    }

    void SoundManager::eraseSounds()
    {
         _sounds.clear();
    }

    void SoundManager::eraseSingleSound(int i)
    {
        if(i>=0 && i< _sounds.size())
        _sounds.erase(_sounds.begin()+i);
    }

    void SoundManager::eraseSingleSound(const std::string &fileName)
    {
        int i = getSound(fileName);
        _sounds.erase (_sounds.begin()+i);
    }

    int SoundManager::getSound(const std::string &fileName) const
    {
        for(unsigned int i=0; i<_sounds.size(); ++i){
            if(fileName==_sounds[i]->getName())
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
        //for(unsigned int i=0; i<_sounds.size(); ++i)
        //    _sounds[i]->setPitch(pitch);
    }
 
    void SoundManager::setLoop(bool loop,const std::string &fileName)
    {
        int i = getSound(fileName);
        _sounds[i]->setLoop(loop);
    }

    void SoundManager::changeLoop(const std::string &fileName)
    {
        int i = getSound(fileName);
        if(_sounds[i]->getLoop())
            _sounds[i]->setLoop(false);
        else
            _sounds[i]->setLoop(true);
    }

    void SoundManager::changeLoop()
    {
        for(unsigned int i=0; i<_sounds.size(); ++i){
            if(_sounds[i]->getLoop())
                _sounds[i]->setLoop(false);
            else
                _sounds[i]->setLoop(true);
        }
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

    void SoundManager::play()
    {
        for(unsigned int i=0; i<_sounds.size(); ++i)
            _sounds[i]->play();
    }

    void SoundManager::pause()
    {
        for(unsigned int i=0; i<_sounds.size(); ++i)
            _sounds[i]->pause();
    }

    void SoundManager::stop()
    {
        for(unsigned int i=0; i<_sounds.size(); ++i){
            _sounds[i]->setLoop(false);
            _sounds[i]->stop();
        }
    }

    void SoundManager::play(const std::string &fileName)
    {
        int i = getSound(fileName);
        _sounds[i]->play();
    }

    void SoundManager::pause(const std::string &fileName)
    {
        int i = getSound(fileName);
        _sounds[i]->pause();
    }

    void SoundManager::stop(const std::string &fileName)
    {
        int i = getSound(fileName);
        _sounds[i]->setLoop(false);
        _sounds[i]->stop();
    }

    void SoundManager::setOffset(const std::string &fileName,Second_t beginT,Second_t endT)
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
        std::fstream file;        
        std::string mainName = fileName + ".dat";
        file.open(mainName, std::ios::in);
        std::string name;
        std::string soundFileName;
        Second_t beginT;
        Second_t endT;
        float volume;
        float pitch;

        while(file >> name){ 
            file >> soundFileName;
            newSound(name,soundFileName);
            //file >> _soundsInfo[_soundsInfo.size()-1]->name;
            file >> beginT;
            file >> endT;
            file >> volume;
            file >> pitch;
            setVolume(volume,name);
            setPitch(pitch,name);
            setOffset(name,beginT,endT);
        }
    }

}
