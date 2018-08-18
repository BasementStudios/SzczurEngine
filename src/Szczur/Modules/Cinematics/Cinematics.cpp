#include "Cinematics.hpp"

namespace rat
{
Cinematics::Cinematics()
{
    LOG_INFO("Initializing Cinematics module");
    initScript();
    auto& w = getModule<Window>();
    w.pushGLStates();
    canvas.create(w.getWindow().getSize().x, w.getWindow().getSize().y);
    w.popGLStates();
    LOG_INFO("Module Cinematics initialized");

}
Cinematics::~Cinematics()
{
    LOG_INFO("Module Cinematics destructed");
}
bool Cinematics::loadFromFile(const char * filename)
{
    endVideo();

    _pFormatCtx = nullptr;
    _pCodecCtx = nullptr;
    _paCodecCtx = nullptr;
    _pCodec = nullptr;
    _paCodec = nullptr;
    _pFrame = nullptr;
    _pFrameRGB = nullptr;
    _optionsDict = nullptr;
    _optionsDictA = nullptr;
    _sws_ctx = nullptr;

    _sound = nullptr;

    _buffer = nullptr;

    _ICurrentLoop = 0;
    _ISmax = 0;

    _alfa = 255;

    av_register_all();

    if(avformat_open_input(&_pFormatCtx, filename, NULL, NULL)!=0)
           return false;

    if(avformat_find_stream_info(_pFormatCtx, NULL)<0)
        return false;

    av_dump_format(_pFormatCtx, 0, filename, 0);
    _isMusic = false;
    _videoStream = -1;
    _audioStream = -1;

    for(int i = 0; i < _pFormatCtx->nb_streams; ++i)
    {
        if(_pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            _videoStream = i;
        }
    }

    for(int i = 0; i < _pFormatCtx->nb_streams; ++i)
    {
        if(_pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            _audioStream = i;
        }
    }

    if(_videoStream < 0)
        return false;

    if(_videoStream >= 0)
    {
        _pCodecCtx = _pFormatCtx->streams[_videoStream]->codec;
        _pCodec = avcodec_find_decoder(_pCodecCtx->codec_id);

        if(avcodec_open2(_pCodecCtx, _pCodec, &_optionsDict)<0)
            return false;
    }
    if(_audioStream >= 0)
    {
        _isMusic = true;
        _paCodecCtx = _pFormatCtx->streams[_audioStream]->codec;
        _paCodec = avcodec_find_decoder(_paCodecCtx->codec_id);

        if(avcodec_open2(_paCodecCtx, _paCodec, &_optionsDictA))
            return false;
    }

    _pFrame = av_frame_alloc();
    _pFrameRGB = av_frame_alloc();
    if (_pFrameRGB == nullptr)
    {
        return false;
    }

    _numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, _pCodecCtx->width, _pCodecCtx->height);
    _buffer = (uint8_t*)av_malloc(_numBytes * sizeof(uint8_t));

    _sws_ctx = sws_getContext(_pCodecCtx->width, _pCodecCtx->height, _pCodecCtx->pix_fmt, _pCodecCtx->width, _pCodecCtx->height, AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);

    avpicture_fill((AVPicture*)_pFrameRGB, _buffer, AV_PIX_FMT_RGB24, _pCodecCtx->width, _pCodecCtx->height);
    return true;
}

void Cinematics::jumpTo(const unsigned int &seekTarget)
{
    if(_sound)
    {
        for(auto p : _sound->g_videoPkts)
        {
            av_free_packet(p);
        }
        _sound->g_videoPkts.clear();
        double fps = av_q2d(_pFormatCtx->streams[_videoStream]->r_frame_rate);
        int64_t seekOnVideo = seekTarget * (fps/1000000);


       // seekOnVideo = av_rescale_q(seekOnVideo, AV_TIME_BASE_Q, _pFormatCtx->streams[_videoStream]->time_base);
        seekOnVideo = seekOnVideo *
             (_pFormatCtx->streams[_videoStream]->time_base.den /
              _pFormatCtx->streams[_videoStream]->time_base.num) /
             (_pFormatCtx->streams[_videoStream]->codec->time_base.den /
              _pFormatCtx->streams[_videoStream]->codec->time_base.num )*
              _pCodecCtx->ticks_per_frame;
       // auto ret = avformat_seek_file(_pFormatCtx, _videoStream, 0, seekOnVideo, seekOnVideo, AVSEEK_FLAG_FRAME);
        auto ret =  av_seek_frame(_pFormatCtx, _videoStream, seekOnVideo, AVSEEK_FLAG_BACKWARD);

        assert(ret >= 0);
        avcodec_flush_buffers(_pCodecCtx);
        _syncAV = true;
    }
}

void Cinematics::play()
{
    _play = true;
    auto &window = getModule<Window>().getWindow();
    for(auto p : _loops)
    {
        p->setFont(_font);
        if(!_isInit) p->init(window.getSize());
    }

    if(!_loops.empty())
    {
        _loops[_ICurrentLoop]->change();
        _jump = _loops[_ICurrentLoop]->change();
    }

    _duration = _pFormatCtx->duration;
    _FrameSize = _pCodecCtx->width * _pCodecCtx->height * 3;

    _data = new sf::Uint8[_pCodecCtx->width * _pCodecCtx->height * 4];

    _lastDecodedTimeStamp = 0;
    _ISmax = 0;

    _im_video.create(_pCodecCtx->width, _pCodecCtx->height);

    _im_video.setSmooth(false);

    _sprite.setTexture(_im_video,true);
    _sprite.setColor(sf::Color(255,255,255,_alfa));
    float x = window.getSize().x;
    float y = window.getSize().y;

    _sprite.setScale(x/_im_video.getSize().x,y/_im_video.getSize().y);


    if(_isMusic)
    {
        _sound = new MovieSound(_pFormatCtx,_audioStream);
        _sound->setVolume(100);
        _sound->play();
    }
    else _sound = new MovieSound();
    _VClock = new sf::Clock;

    _count =  _loops.size();

}

void Cinematics::setFont(sf::Font &font)
{
    _font = font;
}

void Cinematics::setFontPath(const char *filename)
{
    _font.loadFromFile(filename);
}

void Cinematics::addLoop(unsigned int startTime,unsigned int endTime,callme fevent1,const char *text1,int jump1,callme fevent2,const char *text2,int jump2)
{
    std::shared_ptr<VideoLoop> loop = std::make_shared<VideoLoop>(startTime,endTime,fevent1,text1,jump1,fevent2,text2,jump2);
    _loops.push_back(loop);
}
void Cinematics::setTextOnePosition(int x,int y)
{
    _isInit = true;
    for(auto p : _loops)
    {
        p->setPositionOne(sf::Vector2i(x,y));
    }
}

void Cinematics::setTextTwoPosition(int x,int y)
{
    _isInit = true;
    for(auto p : _loops)
    {
        p->setPositionTwo(sf::Vector2i(x,y));
    }
}

void Cinematics::setTextScale(float x,float y)
{
    for(auto p: _loops)
    {
        p->setScale(sf::Vector2f(x,y));
    }
}

void Cinematics::initScript()
{
    Script& script = getModule<Script>();
    auto module = script.newModule("Cinematics");
    module.set_function("addLoop", [this](int startTime, int endTime, sol::function fevent1, const std::string& text1, int jump1, sol::function fevent2, const std::string& text2, int jump2) {
        this->addLoop(startTime, endTime, fevent1, text1.c_str(), jump1, fevent2, text2.c_str(), jump2);
    });

    module.set_function("play", &Cinematics::play, this);
    module.set_function("loadFromFile", &Cinematics::loadFromFile, this);
    module.set_function("setFontPath", &Cinematics::setFontPath, this);
    module.set_function("skip", &Cinematics::skip, this);
    module.set_function("setVolume",&Cinematics::setVolume,this);
    module.set_function("isPlaying",&Cinematics::isPlaying,this);
    module.set_function("setCallbackFinish",&Cinematics::setCallbackFinish,this);
    module.set("onFinish",&Cinematics::_callbackFinish);

}

void Cinematics::update()
{
    if(!_play) return;

    auto &window = getModule<Window>().getWindow();
    auto& w = getModule<Window>();

    bool isDraw = false;


    /* Skipping video */
    if(getModule<Input>().getManager().isPressed(Keyboard::Space))
    {
        skip();
        return;
    } 


    if(_VClock->getElapsedTime().asMicroseconds()>_duration-500000 && _alfa>0)
    {
        _alfa-=5;

        _sprite.setColor(sf::Color(255,255,255,_alfa));

    }

    for(int i=0;i<7;i++)
    {
        _IstartTime = _VClock->getElapsedTime().asMilliseconds();
        if((_ISmax>_sound->timeElapsed()*1000 && !_syncAV&& _ISmax>_duration-4000000)||(_sound->g_audioPkts.empty()&&_ISmax>_duration-4000000))
        {
            if(!_sound->g_videoPkts.empty())
            {
                MyPacket *packet_ptr = _sound->g_videoPkts.front();

                auto decodedLength = avcodec_decode_video2(_pCodecCtx, _pFrame, &_frameFinished, packet_ptr);

                if(!sws_scale(_sws_ctx, (uint8_t const * const *)_pFrame->data, _pFrame->linesize, 0, _pCodecCtx->height, _pFrameRGB->data, _pFrameRGB->linesize))
                {
                    endVideo();
                    return;
                }

                for (int i = 0, j = 0; i < _FrameSize; i += 3, j += 4)
                {
                    _data[j + 0] = _pFrameRGB->data[0][i + 0];
                    _data[j + 1] = _pFrameRGB->data[0][i + 1];
                    _data[j + 2] = _pFrameRGB->data[0][i + 2];
                    _data[j + 3] = 255;
                }

                _im_video.update(_data);

                _IdeltaTime = _VClock->getElapsedTime().asMilliseconds() - _IstartTime;
                if(!_isMusic) sf::sleep((sf::milliseconds(((1000.f/av_q2d(_pFormatCtx->streams[_videoStream]->avg_frame_rate))-_IdeltaTime))));

                packet_ptr->freePacket();
                packet_ptr->free();
                _sound->g_videoPkts.erase(_sound->g_videoPkts.begin());


                return;
            }
            else
            {
                endVideo();
                return;
            }
        }

        if(_ISmax<_sound->timeElapsed()*1000)
        {
            _ISmax = _sound->timeElapsed()*1000;
        }
        if(!_loops.empty())
        {
            if(_loops[_ICurrentLoop] && getModule<Input>().getManager().isPressed(Keyboard::Up)||getModule<Input>().getManager().isPressed(Keyboard::Down))
            {
                _jump = _loops[_ICurrentLoop]->change();
            }
            if(_loops[_ICurrentLoop] && _loops[_ICurrentLoop]->getStartTime()<= _ISmax && getModule<Input>().getManager().isPressed(Keyboard::Return))
            {
                _loops[_ICurrentLoop]->setDraw(false);
                _loops[_ICurrentLoop] = nullptr;
                if(_play == false) return;
                if(_jump!=0)
                {
                    jumpTo(_jump);
                }
                _jump =0;
                _ICurrentLoop++;
                if(_ICurrentLoop==_count)
                {
                    _ICurrentLoop--;
                }
                else
                {
                    _jump = _loops[_ICurrentLoop]->startJump;
                }
                if(_loops[_ICurrentLoop]) _loops[_ICurrentLoop]->setTime(_ISmax);
            }

        }

        MyPacket* packet_ptr = nullptr;

        if(_sound->g_videoPkts.size() < 500)
        {
            packet_ptr = (MyPacket*)av_malloc(sizeof(MyPacket));
            av_init_packet(packet_ptr);

            if(av_read_frame(_pFormatCtx, packet_ptr) < 0)
            {
                if(_sound->g_videoPkts.empty() || _sound->g_audioPkts.empty())
                {
                    //av_free_packet(packet_ptr);
                    //av_free(packet_ptr);
                   // packet_ptr->freePacket();
                   // packet_ptr->free();
                    packet_ptr = nullptr;
                }
                //av_free_packet(packet_ptr);
                //av_free(packet_ptr);
                //packet_ptr->freePacket();
                //packet_ptr->free();
            }
            if(packet_ptr)
            {
                MyPacket& packet = *packet_ptr;
                if(packet.stream_index == _videoStream)
                {
                    _sound->g_videoPkts.push_back(packet_ptr);
                }
                else if(packet.stream_index == _audioStream)
                {
                    if(packet_ptr->pts >= _blockPts && !_syncAV)
                    {
                        std::lock_guard<std::mutex> lk(_sound->g_mut);
                        for(auto p : _audioSyncBuffer)
                        {
                            if(p->pts >= _blockPts)
                            {
                                _sound->g_audioPkts.push_back(p);
                            }
                            else
                            {
                               // av_free_packet(p);
                                //av_free(p);
                                p->freePacket();
                                p->free();
                            }
                        }

                        _sound->g_audioPkts.push_back(packet_ptr);
                        _sound->g_newPktCondition.notify_one();

                        _audioSyncBuffer.clear();
                    }

                    if(_syncAV)
                    {
                        _audioSyncBuffer.push_back(packet_ptr);
                    }

                }
            }

        }
        const auto pStream = _pFormatCtx->streams[_videoStream];
        if(_sound->g_videoPkts.empty()&&_VClock->getElapsedTime().asSeconds()>1)
        {
            endVideo();
            return;
        }
        if(!isDraw &&((_sound->timeElapsed() > _lastDecodedTimeStamp && _sound->isAudioReady())||!_isMusic) && !_sound->g_videoPkts.empty())
        {
            packet_ptr = _sound->g_videoPkts.front();
            _sound->g_videoPkts.pop_front();

            auto decodedLength = avcodec_decode_video2(_pCodecCtx, _pFrame, &_frameFinished, packet_ptr);

            if(_frameFinished)
            {
                sws_scale(_sws_ctx, (uint8_t const * const *)_pFrame->data, _pFrame->linesize, 0, _pCodecCtx->height, _pFrameRGB->data, _pFrameRGB->linesize);

                for (int i = 0, j = 0; i < _FrameSize; i += 3, j += 4)
                {
                    _data[j + 0] = _pFrameRGB->data[0][i + 0];
                    _data[j + 1] = _pFrameRGB->data[0][i + 1];
                    _data[j + 2] = _pFrameRGB->data[0][i + 2];
                    _data[j + 3] = 255;
                }
                _im_video.update(_data);

                isDraw = true;

                int64_t timestamp = av_frame_get_best_effort_timestamp(_pFrame);
                int64_t startTime = pStream->start_time != AV_NOPTS_VALUE ? pStream->start_time : 0;
                int64_t ms = 1000 * (timestamp - startTime) * av_q2d(pStream->time_base);
                _lastDecodedTimeStamp = ms;
                if(_syncAV)
                {
                    _blockPts = ms;
                    _sound->setPlayingOffset(sf::milliseconds(_blockPts));
                    _ISmax = _sound->timeElapsed()*1000;
                    if(!_loops.empty() && _loops[_ICurrentLoop]) _loops[_ICurrentLoop]->setTime(_ISmax);
                    _syncAV = false;
                }

                _IdeltaTime = _VClock->getElapsedTime().asMilliseconds() - _IstartTime;
                if(!_isMusic) sf::sleep((sf::milliseconds(((1000.f/av_q2d(_pFormatCtx->streams[_videoStream]->avg_frame_rate))-_IdeltaTime))));

            }

            if(decodedLength < packet_ptr->size)
            {
                packet_ptr->data += decodedLength;
                packet_ptr->size -= decodedLength;

                _sound->g_videoPkts.push_front(packet_ptr);
            }
            else
            {
               // av_free_packet(packet_ptr);
               // av_free(packet_ptr);
                packet_ptr->freePacket();
                packet_ptr->free();
            }
        }




        if(!_loops.empty() && _loops[_ICurrentLoop])
        {
            int result = _loops[_ICurrentLoop]->update(_ISmax);
            if(result>=0&&!_syncAV)
            {
                jumpTo(result);
            }
        }
    }

}

void Cinematics::render()
{
    if(_play)
    {

        auto& w = getModule<Window>();
        w.pushGLStates();
        canvas.clear({0,0,0,0});

        canvas.draw(_sprite);
        if(!_loops.empty() && _loops[_ICurrentLoop]) _loops[_ICurrentLoop]->draw(canvas);

        canvas.display();
        w.draw(sf::Sprite(canvas.getTexture()));
        w.popGLStates();
    }
}

void Cinematics::skip()
{
    if(_play)
    {
        _sound->setVolume(0);
        for(auto p : _sound->g_videoPkts)
        {
            p->freePacket();
            p->free();
        }
        _sound->g_videoPkts.clear();
        if(_isMusic)
        {
            _sound->stop();
            avcodec_close(_paCodecCtx);
        }
        sws_freeContext(_sws_ctx);
        av_free(_buffer);
        av_free(_pFrameRGB);
        av_free(_pFrame);
        avcodec_close(_pCodecCtx);

        avformat_close_input(&_pFormatCtx);

        delete [] _data;
        _play = false;

        if(_callbackFinish.valid() && _isCallbackSet)
        {
            _callbackFinish();
            _isCallbackSet = false;
        }
    }
}


void Cinematics::endVideo()
{
    if(_play)
    {
        for(auto p : _sound->g_videoPkts)
        {
            p->freePacket();
            p->free();
        }
        _sound->g_videoPkts.clear();

        if(_isMusic)
        { 
            _sound->stop();
            avcodec_close(_paCodecCtx);
        }
               
        sws_freeContext(_sws_ctx);
        av_free(_buffer);
        av_free(_pFrameRGB);
        av_free(_pFrame);
        avcodec_close(_pCodecCtx);

        avformat_close_input(&_pFormatCtx);

        delete [] _data;
        _play = false;

        if(_callbackFinish.valid() && _isCallbackSet)
        {
            _callbackFinish();
            _isCallbackSet = false;
        }
    }
}

void Cinematics::setVolume(float vol)
{
    _sound->setVolume(vol);
}

bool Cinematics::isPlaying()
{
    return _play;
}

void Cinematics::setCallbackFinish(callme t)
{
    _callbackFinish = t;
    _isCallbackSet = true;
}

}
