#include "DialogSoundBase.hpp"

namespace rat
{
        bool DialogSoundBase::init(const std::string& fileName)
        {
            _name = fileName;
            if (!loadBuffer())
                return false;

            _length=buffer.getDuration().asSeconds();
            return true;
        };

        float DialogSoundBase::getVolume()
        {
            return _volume;
        }

        void DialogSoundBase::setVolume(float volume)
        {
            sound.setVolume(volume);
        }

        void DialogSoundBase::setBaseVolume(float volume)
        {
            _volume = volume;
            sound.setVolume(volume);
        }

        void DialogSoundBase::setPlayingOffset(float seconds)
        {
            _offset=seconds;
            sound.setPlayingOffset(sf::seconds(seconds));

            for (unsigned int i=0; i<_offsets.size(); ++i){
                if(seconds >= _offsets[i].beginTime && seconds < _offsets[i].endTime){
                    _currentOffsetID = i;
                    break;
                }
            }
        }

        void DialogSoundBase::setOffset(int offset)
        {
            if (offset>=_offsets.size()||offset<0)
                return;

            _offset=_offsets[offset].beginTime;
            sound.setPlayingOffset(sf::seconds(_offset));
            _currentOffsetID=offset;
            play();
        }

        void DialogSoundBase::repeat()
        {
            _offset=_offsets[_currentOffsetID].beginTime;
            sound.setPlayingOffset(sf::seconds(_offset));

            play();
        }
        void DialogSoundBase::next()
        {
            _currentOffsetID++;
            _offset=_offsets[_currentOffsetID].beginTime;
            sound.setPlayingOffset(sf::seconds(_offset));

            play();
        }

        const std::string DialogSoundBase::chooseOption(int option)
        {
            if(option<0||option>=_offsets.size())
                return "";

            if(_offsets[_currentOffsetID]._options[option]._file==""){
                setOffset(_offsets[_currentOffsetID]._options[option].offsetNumber);
            }else{
                return _offsets[_currentOffsetID]._options[option]._file;
            }

            return "";
        }

        int DialogSoundBase::getOptionOffset(int option)
        {
             if(option<0||option>=_offsets.size())
                return 0;
            return _offsets[_currentOffsetID]._options[option].offsetNumber;
        }

        void DialogSoundBase::addOption(int offset, int newOffset,  const std::string &fileName)
        {
            if(offset<0||offset>_offsets.size())
                return;
            _offsets[offset]._options.emplace_back();
            _offsets[offset]._options[_offsets[offset]._options.size()-1].offsetNumber=newOffset;
            _offsets[offset]._options[_offsets[offset]._options.size()-1]._file=fileName;
        }

        float DialogSoundBase::getOffset()
        {
            return sound.getPlayingOffset().asSeconds();
        }

        int DialogSoundBase::getOffsetID()
        {
            return _currentOffsetID;
        }

        void DialogSoundBase::addOffset(float beginT, float endT)
        {
            offset offset;
            offset.beginTime=beginT;
            if(endT==-1)
                offset.endTime=_length;
            else
                offset.endTime=endT;

            _offsets.push_back(offset);
        }

        void DialogSoundBase::addCallback(float seconds, const std::function<void(void)>& cback)
        {
            callback callback;
            callback.time=seconds;
            callback.cback=cback;

            _callbacks.push_back(callback);
        }

        void DialogSoundBase::eraseCallbacks()
        {
            _callbacks.clear();
            _currentCallback = 0;
        }

        bool DialogSoundBase::soundEnd()
        {
            if(getOffset()>=_length){
                restart();
                return true;
            }
            return false;
        }

        float DialogSoundBase::getPitch()
        {
            return sound.getPitch();
        }

        void DialogSoundBase::setPitch(float pitch)
        {
            _pitch = pitch;
            sound.setPitch(pitch);
        }

        bool DialogSoundBase::getLoop()
        {
            return sound.getLoop();
        }

        void DialogSoundBase::setLoop(bool loop)
        {
            sound.setLoop(loop);
        }

        void DialogSoundBase::play()
        {
            sound.setBuffer(buffer);
            sound.play();
            sound.setPlayingOffset(sf::seconds(_offset));
        }

        sf::SoundSource::Status DialogSoundBase::update()
        {
            if (_offsets.size()<=0)
                return sound.getStatus();

            if(getOffset()>=_length){
                restart();
                return sound.getStatus();
            }

            if (_currentOffsetID>_offsets.size())
                _currentOffsetID = 0;

            if(_callbacks.size() > 0 && _currentCallback < _callbacks.size()){

                if((int)getOffset()==(int)_callbacks[_currentCallback].time){
                        std::invoke(_callbacks[_currentCallback].cback);
                        _currentCallback++;
                    }
            }

            if (getOffset()>=_offsets[_currentOffsetID].endTime)
            {
                sound.pause();
            }
            
            return sound.getStatus();
        }

        void DialogSoundBase::restart(int offset)
        {
            if (offset<0||offset>=_offsets.size())
                _offset = 0;
            else
                _offset = _offsets[offset].beginTime;

                _currentOffsetID = offset;
                _currentCallback = 0;
                sound.stop();
        }

        const std::string& DialogSoundBase::getName()
        {
            return _name;
        }

        bool DialogSoundBase::loadBuffer()
        {
            return buffer.loadFromFile(getPath());
        }

        std::string DialogSoundBase::getPath()
        {
            return _name + ".flac";
        }
}
