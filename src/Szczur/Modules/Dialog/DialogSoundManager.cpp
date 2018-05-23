#include "DialogSoundManager.hpp"

namespace rat
{
        bool DialogSoundManager::load(const std::string& fileName)
        {
            _name = fileName;
            if (!loadBuffer())
                return false;

            _length = buffer.getDuration().asSeconds();
            return true;
        };

        float DialogSoundManager::getVolume()
        {
            return _volume;
        }

        void DialogSoundManager::setVolume(float volume)
        {
            sound.setVolume(volume);
        }

        void DialogSoundManager::setBaseVolume(float volume)
        {
            _volume = volume;
            sound.setVolume(volume);
        }

        void DialogSoundManager::setPlayingOffset(Second_t seconds)
        {
            _offset = seconds;
            sound.setPlayingOffset(sf::seconds(seconds));

            for (unsigned int i = 0; i < _offsets.size(); ++i){
                if(seconds >= _offsets[i]->beginTime && seconds < _offsets[i]->endTime){
                    _currentOffsetID = i;
                    break;
                }
            }
        }

        void DialogSoundManager::setOffsetID(int offset)
        {
            if (offset >= _offsets.size() || offset < 0)
                return;

            _offset = _offsets[offset]->beginTime;
            sound.setPlayingOffset(sf::seconds(_offset));
            _currentOffsetID = offset;
            play();
        }

        void DialogSoundManager::pause()
        {
            _pauseTime = getOffset();
            sound.pause();
        }

        void DialogSoundManager::unPause()
        {
            sound.play();
            sound.setPlayingOffset(sf::seconds(_pauseTime));
        }

        void DialogSoundManager::repeat()
        {
            _offset = _offsets[_currentOffsetID]->beginTime;
            sound.setPlayingOffset(sf::seconds(_offset));

            play();
        }
        void DialogSoundManager::next()
        {
            _currentOffsetID++;
            _offset = _offsets[_currentOffsetID]->beginTime;
            sound.setPlayingOffset(sf::seconds(_offset));

            play();
        }

        const std::string DialogSoundManager::chooseOption(int option)
        {
            if(option < 0 || option >= _offsets.size())
                return "";

            if(_offsets[_currentOffsetID]->_options[option]._file == ""){
                setOffsetID(_offsets[_currentOffsetID]->_options[option].offsetNumber);
            }else{
                return _offsets[_currentOffsetID]->_options[option]._file;
            }

            return "";
        }

        int DialogSoundManager::getOptionOffset(int option)
        {
            if(option < 0 || option >= _offsets.size())
                return 0;
            return _offsets[_currentOffsetID]->_options[option].offsetNumber;
        }

        void DialogSoundManager::addOption(int offset, int newOffset,  const std::string &fileName)
        {
            if(offset < 0||offset > _offsets.size())
                return;
            _offsets[offset]->_options.emplace_back();
            _offsets[offset]->_options[_offsets[offset]->_options.size()-1].offsetNumber=newOffset;
            _offsets[offset]->_options[_offsets[offset]->_options.size()-1]._file=fileName;
        }

        DialogSoundManager::Second_t DialogSoundManager::getOffset()
        {
            return sound.getPlayingOffset().asSeconds();
        }

        int DialogSoundManager::getOffsetID()
        {
            return _currentOffsetID;
        }

        void DialogSoundManager::addOffset(Second_t beginT, Second_t endT)
        {
            std::shared_ptr<DialogSoundManager::offset> offset(new DialogSoundManager::offset);
            offset->beginTime = beginT;

            if(endT == -1)
                offset->endTime = _length;
            else
                offset->endTime = endT;

            _offsets.push_back(offset);
        }

        void DialogSoundManager::addCallback(Second_t seconds, const std::function<void(void)>& cback)
        {
            std::shared_ptr<DialogSoundManager::callback> callback(new DialogSoundManager::callback);
            callback->time = seconds;
            callback->cback = cback;

            _callbacks.push_back(callback);
        }

        void DialogSoundManager::eraseCallbacks()
        {
            _callbacks.clear();
            _currentCallback = 0;
        }

        bool DialogSoundManager::soundEnd()
        {
            if(getOffset() >= _length){
                restart();
                return true;
            }
            return false;
        }

        float DialogSoundManager::getPitch() const
        {
            return sound.getPitch();
        }

        void DialogSoundManager::setPitch(float pitch)
        {
            _pitch = pitch;
            sound.setPitch(pitch);
        }

        bool DialogSoundManager::getLoop() const
        {
            return sound.getLoop();
        }

        void DialogSoundManager::setLoop(bool loop)
        {
            sound.setLoop(loop);
        }

        void DialogSoundManager::play()
        {
            sound.setBuffer(buffer);
            sound.play();
            sound.setPlayingOffset(sf::seconds(_offset));
        }

        void DialogSoundManager::skip() {
            if(sound.getStatus() != sf::SoundSource::Status::Paused) {
                if(_currentCallback >= _callbacks.size()) {
                    sound.pause();
                    _currentCallback = 0;
                    return;
                }
                std::invoke(_callbacks[_currentCallback]->cback);
                setPlayingOffset(_callbacks[_currentCallback]->time);
                ++_currentCallback;
            }
        }

        sf::SoundSource::Status DialogSoundManager::update()
        {
            if (_offsets.size() <= 0)
                return sound.getStatus();

            if(getOffset() >= _length){
                restart();
                return sound.getStatus();
            }

            if (_currentOffsetID > _offsets.size())
                _currentOffsetID = 0;

            if(_callbacks.size() > 0 && _currentCallback >=0 && _currentCallback < _callbacks.size()){
                if(getOffset() == _callbacks[_currentCallback]->time){
                        std::invoke(_callbacks[_currentCallback]->cback);
                        _currentCallback++;
                    }
            }

            if (getOffset() >= _offsets[_currentOffsetID]->endTime)
            {
                sound.pause();
            }

            return sound.getStatus();
        }

        void DialogSoundManager::restart(int offset)
        {
            if (offset < 0 || offset >= _offsets.size())
                _offset = 0;
            else
                _offset = _offsets[offset]->beginTime;

            _currentOffsetID = offset;
            _currentCallback = 0;
            sound.stop();
        }

        const std::string& DialogSoundManager::getName() const
        {
            return _name;
        }

        bool DialogSoundManager::loadBuffer()
        {
            return buffer.loadFromFile(getPath());
        }

        std::string DialogSoundManager::getPath() const
        {
            return _name + ".flac";
        }
}
