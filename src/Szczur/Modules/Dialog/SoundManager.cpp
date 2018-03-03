#include "SoundManager.hpp"

namespace rat
{
        void SoundManager::newSound(const std::string &fileName)
        {
            if(getSound(fileName)!=_sounds.size())
                return;
            std::shared_ptr<SoundBase> sound(new SoundBase);
            if (sound->init(fileName))
                _sounds.push_back(sound);
        }

        void SoundManager::removeSound(const std::string &fileName)
        {
            int i = getID(fileName);
            if(_currentSoundID==i){
                restart();
                _currentSoundID=0;
             }
            _sounds.erase(_sounds.begin() + i);
        }

        int SoundManager::getSound(const std::string &fileName)
        {
            for (unsigned int i=0; i<_sounds.size(); ++i){
                    if(fileName == _sounds[i]->getName())
                       return i;
            }

            return _sounds.size();
        }

        int SoundManager::getID(const std::string &fileName)
        {
            if (fileName==""||getSound(fileName)<0||getSound(fileName)>=_sounds.size())
                return _currentSoundID;
            else
                return getSound(fileName);
        }

        void SoundManager::changeSound(const std::string &fileName)
        {
            restart("",-1);
            _currentSoundID = getSound(fileName);
        }

        void SoundManager::chooseOption(int option, const std::string &fileName)
        {
            int i = getID(fileName);
            int offset = _sounds[i]->getOptionOffset(option);
            std::string newSound = _sounds[i]->chooseOption(option);
            if( newSound != ""){
                changeSound( newSound);
            }
            _sounds[_currentSoundID]->setOffset(offset);
              play();
        }

        void SoundManager::addOption(int offset, int newOffset,  const std::string &newfileName,const std::string &fileName)
        {
             int i = getID(fileName);
             _sounds[i]->addOption(offset,newOffset,newfileName);
        }

        void SoundManager::setVolume(float volume, const std::string &fileName)
        {
            if (fileName==""){
                _volumeGame = volume;
                for (unsigned int i=0; i<_sounds.size(); ++i)
                    _sounds[i]->setVolume(_sounds[i]->getVolume()*(volume/100));
            }
            else{
                int i = _currentSoundID;
                _sounds[i]->setBaseVolume(volume*(_volumeGame/100));
            }
        }

        void SoundManager::setPitch(float pitch, const std::string &fileName)
        {
            int i = getID(fileName);
            _sounds[i]->setPitch(pitch);
        }

        void SoundManager::setLoop(bool loop,const std::string &fileName)
        {
            int i = getSound(fileName);
            _sounds[i]->setLoop(loop);
        }

        void SoundManager::changeLoop(const std::string &fileName)
        {
            int i = getSound(fileName);

            if (_sounds[i]->getLoop())
                _sounds[i]->setLoop(false);
            else
                _sounds[i]->setLoop(true);
        }

        void SoundManager::setPlayingOffset(float seconds,const std::string &fileName)
        {
            int i = getID(fileName);
            _sounds[i]->setPlayingOffset(seconds);
        }

        void SoundManager::setOffsetID(int offset,const std::string &fileName)
        {
            int i = getID(fileName);
            _sounds[i]->setOffset(offset);
        }

        float SoundManager::getOffset(const std::string &fileName)
        {
            int i = getID(fileName);
            return _sounds[i]->getOffset();
        }

        int SoundManager::getOffsetID(const std::string &fileName)
        {
            int i = getID(fileName);
            return _sounds[i]->getOffsetID();
        }

        void SoundManager::addOffset(float beginT,float endT,const std::string &fileName)
        {
            int i = getID(fileName);
            _sounds[i]->addOffset(beginT,endT);
        }

        void SoundManager::repeat(const std::string &fileName)
        {
            int i = getID(fileName);
            _sounds[i]->repeat();
        }

        void SoundManager::next(const std::string &fileName)
        {
            int i = getID(fileName);
            _sounds[i]->next();
        }

        bool SoundManager::soundEnd(const std::string &fileName)
        {
            int i = getID(fileName);
            return _sounds[i]->soundEnd();
        }

        void SoundManager::restart(const std::string &fileName,int offset)
        {
            int i = getID(fileName);
            _sounds[i]->restart(offset);
        }

        sf::SoundSource::Status SoundManager::update(const std::string &fileName)
        {
            int i = getID(fileName);
            return _sounds[i]->update();
        }

        void SoundManager::play(const std::string &fileName)
        {
            int i = getID(fileName);
            _sounds[i]->play();
        }
}
