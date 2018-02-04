#include "SoundManager.hpp"

namespace rat
{
        void SoundManager::newSound(const std::string &fileName)
        {
            SoundBase sound;
            if (sound.init(fileName))
                _sounds.push_back(sound);
        }

        int SoundManager::getSound(const std::string &fileName)
        {
            for (unsigned int i=0; i<_sounds.size(); ++i){
                    if(fileName == _sounds[i].getName())
                       return i;
            }

            return _currentSoundID;
        }

        int SoundManager::getID(const std::string &fileName)
        {
            if (fileName=="")
                return _currentSoundID;
            else
                return getSound(fileName);
        }

        void SoundManager::changeSound(const std::string &fileName)
        {
            restart("",-1);
            _currentSoundID = getSound(fileName);
        }

        void SoundManager::setVolume(float volume, const std::string &fileName)
        {
            if (fileName==""){
                _volumeGame = volume;
                for (unsigned int i=0; i<_sounds.size(); ++i)
                    _sounds[i].setVolume(_sounds[i].getVolume()*(volume/100));
            }
            else{
                int i = _currentSoundID;
                _sounds[i].setBaseVolume(volume*(_volumeGame/100));
            }
        }

        void SoundManager::setPitch(float pitch, const std::string &fileName)
        {
            int i = getID(fileName);
            _sounds[i].setPitch(pitch);
        }

        void SoundManager::setLoop(bool loop,const std::string &fileName)
        {
            int i = getSound(fileName);
            _sounds[i].setLoop(loop);
        }

        void SoundManager::changeLoop(const std::string &fileName)
        {
            int i = getSound(fileName);

            if (_sounds[i].getLoop())
                _sounds[i].setLoop(false);
            else
                _sounds[i].setLoop(true);
        }

        void SoundManager::setPlayingOffset(float seconds,const std::string &fileName)
        {
            int i = getID(fileName);
            _sounds[i].setPlayingOffset(seconds);
        }

        void SoundManager::setOffsetID(int offset,const std::string &fileName)
        {
            int i = getID(fileName);
            _sounds[i].setOffset(offset);
        }

        float SoundManager::getOffset(const std::string &fileName)
        {
            int i = getID(fileName);
            return _sounds[i].getOffset();
        }

        int SoundManager::getOffsetID(const std::string &fileName)
        {
            int i = getID(fileName);
            return _sounds[i].getOffsetID();
        }

        void SoundManager::addOffset(float beginT,float endT,const std::string &fileName)
        {
            int i = getID(fileName);
            _sounds[i].addOffset(beginT,endT);
        }

        void SoundManager::repeat(const std::string &fileName)
        {
            int i = getID(fileName);
            _sounds[i].repeat();
        }

        void SoundManager::next(const std::string &fileName)
        {
            int i = getID(fileName);
            _sounds[i].next();
        }

        bool SoundManager::soundEnd(const std::string &fileName)
        {
            int i = getID(fileName);
            return _sounds[i].soundEnd();
        }

        void SoundManager::restart(const std::string &fileName,int offset)
        {
            int i = getID(fileName);
            _sounds[i].restart(offset);
        }

        sf::SoundSource::Status SoundManager::update(const std::string &fileName)
        {
            int i = getID(fileName);
            return _sounds[i].update();
        }

        void SoundManager::play(const std::string &fileName)
        {
            int i = getID(fileName);
            _sounds[i].play();
        }
}
