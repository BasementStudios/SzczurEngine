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
    av_register_all();
  
    if(avformat_open_input(&m_pFormatCtx, filename, NULL, NULL)!=0)   
           return false;     

    if(avformat_find_stream_info(m_pFormatCtx, NULL)<0)
        return false; 
    
    av_dump_format(m_pFormatCtx, 0, filename, 0);
    
    m_videoStream = -1;
    m_audioStream = -1;
    for(int i = 0; i < m_pFormatCtx->nb_streams; ++i)
    {
        if(m_pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            m_videoStream = i;
        }
        else if(m_pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            m_audioStream = i;
        }
    }
    
    if(m_videoStream < 0)
        return false;

    if(m_videoStream >= 0)
    {
        m_pCodecCtx = m_pFormatCtx->streams[m_videoStream]->codec;
        m_pCodec = avcodec_find_decoder(m_pCodecCtx->codec_id);
        
        if(avcodec_open2(m_pCodecCtx, m_pCodec, &m_optionsDict)<0)
            return false;
    }
    if(m_audioStream >= 0)
    {
        m_isMusic = true;
        m_paCodecCtx = m_pFormatCtx->streams[m_audioStream]->codec;
        m_paCodec = avcodec_find_decoder(m_paCodecCtx->codec_id);
        
        if(avcodec_open2(m_paCodecCtx, m_paCodec, &m_optionsDictA))
            return false;
    }

    m_pFrame = av_frame_alloc();
    m_pFrameRGB = av_frame_alloc();
    if (m_pFrameRGB == nullptr)
    {
        return false;
    }

    m_numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, m_pCodecCtx->width, m_pCodecCtx->height);
    m_buffer = (uint8_t*)av_malloc(m_numBytes * sizeof(uint8_t));
    
    m_sws_ctx = sws_getContext(m_pCodecCtx->width, m_pCodecCtx->height, m_pCodecCtx->pix_fmt, m_pCodecCtx->width, m_pCodecCtx->height, AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);

    avpicture_fill((AVPicture*)m_pFrameRGB, m_buffer, AV_PIX_FMT_RGB24, m_pCodecCtx->width, m_pCodecCtx->height);
    return true;
}

void Cinematics::jumpTo(const unsigned int &seekTarget)
{
    if(m_sound)
    {
        for(auto p : m_sound->g_videoPkts)
        {
            av_free_packet(p);
        }
        m_sound->g_videoPkts.clear();
        double fps = av_q2d(m_pFormatCtx->streams[m_videoStream]->r_frame_rate);
        int64_t seekOnVideo = seekTarget * (fps/1000000);


       // seekOnVideo = av_rescale_q(seekOnVideo, AV_TIME_BASE_Q, m_pFormatCtx->streams[m_videoStream]->time_base);
        seekOnVideo = seekOnVideo *
             (m_pFormatCtx->streams[m_videoStream]->time_base.den /
              m_pFormatCtx->streams[m_videoStream]->time_base.num) /
             (m_pFormatCtx->streams[m_videoStream]->codec->time_base.den /
              m_pFormatCtx->streams[m_videoStream]->codec->time_base.num )*
              m_pCodecCtx->ticks_per_frame;
       // auto ret = avformat_seek_file(m_pFormatCtx, m_videoStream, 0, seekOnVideo, seekOnVideo, AVSEEK_FLAG_FRAME);
        auto ret =  av_seek_frame(m_pFormatCtx, m_videoStream, seekOnVideo, AVSEEK_FLAG_BACKWARD);
        
        assert(ret >= 0);
        avcodec_flush_buffers(m_pCodecCtx);
        m_syncAV = true;
    }
}

void Cinematics::play()
{
    m_play = true;
    auto &window = getModule<Window>().getWindow(); 
    for(auto p : m_loops)
    {
        p->setFont(m_font);
        if(!m_isInit) p->init(window.getSize());
    }
   
    if(!m_loops.empty())
    {
        m_loops[m_ICurrentLoop]->change();
        m_jump = m_loops[m_ICurrentLoop]->change();
    }
 
    m_duration = m_pFormatCtx->duration;
    m_FrameSize = m_pCodecCtx->width * m_pCodecCtx->height * 3;

    m_data = new sf::Uint8[m_pCodecCtx->width * m_pCodecCtx->height * 4];
    
   
    m_im_video.create(m_pCodecCtx->width, m_pCodecCtx->height);
    m_im_video.setSmooth(false);

    m_sprite.setTexture(m_im_video);
    float x = window.getSize().x;
    float y = window.getSize().y;
    m_sprite.setScale(x/m_im_video.getSize().x,y/m_im_video.getSize().y);
   
    if(m_isMusic) 
    {
        m_sound = new MovieSound(m_pFormatCtx,m_audioStream);
        m_sound->play();
    }
    else m_sound = new MovieSound();
    m_VClock = new sf::Clock;

    m_count =  m_loops.size();
    
}

void Cinematics::setFont(sf::Font &font)
{
    m_font = font;
}

void Cinematics::setFontPath(const char *filename)
{
    m_font.loadFromFile(filename);
}

void Cinematics::addLoop(unsigned int startTime,unsigned int endTime,callme fevent1,const char *text1,int jump1,callme fevent2,const char *text2,int jump2)
{
    std::shared_ptr<VideoLoop> loop = std::make_shared<VideoLoop>(startTime,endTime,fevent1,text1,jump1,fevent2,text2,jump2);
    m_loops.push_back(loop);
}
void Cinematics::setTextOnePosition(int x,int y)
{
    m_isInit = true;
    for(auto p : m_loops)
    {
        p->setPositionOne(sf::Vector2i(x,y));
    }
}

void Cinematics::setTextTwoPosition(int x,int y)
{
    m_isInit = true;
    for(auto p : m_loops)
    {
        p->setPositionTwo(sf::Vector2i(x,y));
    }
}

void Cinematics::setTextScale(float x,float y)
{
    for(auto p: m_loops)
    {
        p->setScale(sf::Vector2f(x,y));
    }
}

void Cinematics::initScript()
{
    Script& script = getModule<Script>();
    auto module = script.newModule("Cinematics");
    module.set_function("addLoop", [this](int startTime, int endTime, sol::function fevent1, const std::string& text1, int jump1, sol::function fevent2, const std::string& text2, int jump2) {
        this->addLoop(startTime, endTime, std::function<void()>(fevent1), text1.c_str(), jump1, std::function<void()>(fevent2), text2.c_str(), jump2);
    });

    module.set_function("play", &Cinematics::play, this);
    module.set_function("loadFromFile", &Cinematics::loadFromFile, this);
    module.set_function("setFontPath", &Cinematics::setFontPath, this);
    module.set_function("stop", &Cinematics::stop, this);
    module.set_function("setVolume",&Cinematics::setVolume,this);
    module.set_function("isPlaying",&Cinematics::isPlaying,this);

}

void Cinematics::update()
{
    if(!m_play) return;
    
    auto &window = getModule<Window>().getWindow(); 
    auto& w = getModule<Window>();

    bool isDraw = false;

    for(int i=0;i<7;i++)
    {
        if((m_ISmax>m_sound->timeElapsed()*1000 && !m_syncAV&& m_ISmax>m_duration-4000000)||(m_sound->g_audioPkts.empty()&&m_ISmax>m_duration-4000000))
        {
            if(!m_sound->g_videoPkts.empty())
            {
                m_IstartTime = m_VClock->getElapsedTime().asMilliseconds();
                AVPacket *packet_ptr = m_sound->g_videoPkts.front();
 
                auto decodedLength = avcodec_decode_video2(m_pCodecCtx, m_pFrame, &m_frameFinished, packet_ptr);
    
                if(!sws_scale(m_sws_ctx, (uint8_t const * const *)m_pFrame->data, m_pFrame->linesize, 0, m_pCodecCtx->height, m_pFrameRGB->data, m_pFrameRGB->linesize))
                {
                    m_sound->g_videoPkts.clear();
                    return;
                }  
                            
                for (int i = 0, j = 0; i < m_FrameSize; i += 3, j += 4)
                {
                    m_data[j + 0] = m_pFrameRGB->data[0][i + 0];
                    m_data[j + 1] = m_pFrameRGB->data[0][i + 1];
                    m_data[j + 2] = m_pFrameRGB->data[0][i + 2];
                    m_data[j + 3] = 255;
                }
                
                m_im_video.update(m_data);
                
                m_IdeltaTime = m_VClock->getElapsedTime().asMilliseconds() - m_IstartTime;
                //sf::sleep(sf::milliseconds((1000.f/av_q2d(m_pFormatCtx->streams[m_videoStream]->avg_frame_rate))-m_IdeltaTime));
                m_sound->g_videoPkts.erase(m_sound->g_videoPkts.begin());
                return;
            }
            else
            {
                //stop(); music ended, we don't have to clear anything
                m_play = false;
                return;
            }
        }

        if(m_ISmax<m_sound->timeElapsed()*1000)
        {
            m_ISmax = m_sound->timeElapsed()*1000;
        }
        if(!m_loops.empty()) 
        { 
            if(m_loops[m_ICurrentLoop] && getModule<Input>().getManager().isPressed(Keyboard::Up)||getModule<Input>().getManager().isPressed(Keyboard::Down))
            {
                m_jump = m_loops[m_ICurrentLoop]->change();
            }
            if(m_loops[m_ICurrentLoop] && m_loops[m_ICurrentLoop]->getStartTime()<= m_ISmax && getModule<Input>().getManager().isPressed(Keyboard::Return))
            {
                m_loops[m_ICurrentLoop]->setDraw(false);
                m_loops[m_ICurrentLoop] = nullptr;
                if(m_play == false) return;
                if(m_jump!=0) 
                {
                    jumpTo(m_jump);
                }
                m_jump =0;
                m_ICurrentLoop++;
                if(m_ICurrentLoop==m_count)
                {
                    m_ICurrentLoop--;
                }
                else
                {
                    m_jump = m_loops[m_ICurrentLoop]->startJump;
                }
                if(m_loops[m_ICurrentLoop]) m_loops[m_ICurrentLoop]->setTime(m_ISmax);
            }
            
        }
        
        AVPacket* packet_ptr = nullptr;
        
        if(m_sound->g_videoPkts.size() < 500)
        {
            packet_ptr = (AVPacket*)av_malloc(sizeof(AVPacket));
            av_init_packet(packet_ptr);
            
            if(av_read_frame(m_pFormatCtx, packet_ptr) < 0)
            {
                if(m_sound->g_videoPkts.empty() || m_sound->g_audioPkts.empty())
                {
                    av_free_packet(packet_ptr);
                    av_free(packet_ptr);
                    packet_ptr = nullptr;
                }
                av_free_packet(packet_ptr);
                av_free(packet_ptr);
            }
            if(packet_ptr)
            {
                AVPacket& packet = *packet_ptr;
                if(packet.stream_index == m_videoStream)
                {
                    m_sound->g_videoPkts.push_back(packet_ptr);
                }
                else if(packet.stream_index == m_audioStream)
                {
                    if(packet_ptr->pts >= m_blockPts && !m_syncAV)
                    {
                        std::lock_guard<std::mutex> lk(m_sound->g_mut);
                        for(auto p : m_audioSyncBuffer)
                        {
                            if(p->pts >= m_blockPts)
                            {
                                m_sound->g_audioPkts.push_back(p);
                            }
                            else
                            {
                                av_free_packet(p);
                                av_free(p);
                            }
                        }
                    
                        m_sound->g_audioPkts.push_back(packet_ptr);
                        m_sound->g_newPktCondition.notify_one();
                           
                        m_audioSyncBuffer.clear();
                    }
                    
                    if(m_syncAV)
                    {
                        m_audioSyncBuffer.push_back(packet_ptr);
                    }
                    
                }
            }
   
        }
        const auto pStream = m_pFormatCtx->streams[m_videoStream];
        if(!isDraw &&((m_sound->timeElapsed() > m_lastDecodedTimeStamp && m_sound->isAudioReady())||!m_isMusic) && !m_sound->g_videoPkts.empty())
        {
            packet_ptr = m_sound->g_videoPkts.front();
            m_sound->g_videoPkts.pop_front();
            
            auto decodedLength = avcodec_decode_video2(m_pCodecCtx, m_pFrame, &m_frameFinished, packet_ptr);
            
            if(m_frameFinished)
            {
                sws_scale(m_sws_ctx, (uint8_t const * const *)m_pFrame->data, m_pFrame->linesize, 0, m_pCodecCtx->height, m_pFrameRGB->data, m_pFrameRGB->linesize);
                
                for (int i = 0, j = 0; i < m_FrameSize; i += 3, j += 4)
                {
                    m_data[j + 0] = m_pFrameRGB->data[0][i + 0];
                    m_data[j + 1] = m_pFrameRGB->data[0][i + 1];
                    m_data[j + 2] = m_pFrameRGB->data[0][i + 2];
                    m_data[j + 3] = 255;
                }
                m_im_video.update(m_data);
               
                isDraw = true;
        
                int64_t timestamp = av_frame_get_best_effort_timestamp(m_pFrame);
                int64_t startTime = pStream->start_time != AV_NOPTS_VALUE ? pStream->start_time : 0;
                int64_t ms = 1000 * (timestamp - startTime) * av_q2d(pStream->time_base);
                m_lastDecodedTimeStamp = ms;
                if(m_syncAV)
                {
                    m_blockPts = ms;
                    m_sound->setPlayingOffset(sf::milliseconds(m_blockPts));
                    m_ISmax = m_sound->timeElapsed()*1000;
                    if(!m_loops.empty() && m_loops[m_ICurrentLoop]) m_loops[m_ICurrentLoop]->setTime(m_ISmax);
                    m_syncAV = false;
                }
              
                
            }
           
            if(decodedLength < packet_ptr->size)
            {
                packet_ptr->data += decodedLength;
                packet_ptr->size -= decodedLength;
                
                m_sound->g_videoPkts.push_front(packet_ptr);
            }
            else
            {
                av_free_packet(packet_ptr);
                av_free(packet_ptr);
            }
        }
        
        
       

        if(!m_loops.empty() && m_loops[m_ICurrentLoop]) 
        {
            int result = m_loops[m_ICurrentLoop]->update(m_ISmax);
            if(result>=0&&!m_syncAV)
            {
                jumpTo(result);
            }
        }
    }

}

void Cinematics::render()
{
    if(m_play)
    {
        auto& w = getModule<Window>();
        w.pushGLStates();
        canvas.clear({0,0,0,0});
                    
        canvas.draw(m_sprite);
        if(!m_loops.empty() && m_loops[m_ICurrentLoop]) m_loops[m_ICurrentLoop]->draw(canvas);
                    
        canvas.display();
        w.draw(sf::Sprite(canvas.getTexture()));
        w.popGLStates();
    }
}

void Cinematics::stop()
{
    if(m_play)
    {
        m_sound->stop();
        for(auto p : m_sound->g_videoPkts)
        {
            av_free_packet(p);
        }
        for(auto p : m_sound->g_audioPkts)
        {
            av_free_packet(p);
            av_free(p);
        }
        sws_freeContext(m_sws_ctx);
        av_free(m_buffer);
        av_free(m_pFrameRGB);
        av_free(m_pFrame);
        avcodec_close(m_pCodecCtx);
        avcodec_close(m_paCodecCtx);
        avformat_close_input(&m_pFormatCtx);
        
        delete [] m_data;
        m_play = false;

    }
}

void Cinematics::setVolume(float vol)
{
    m_sound->setVolume(vol);
}

bool Cinematics::isPlaying()
{
    return m_play;
}

}