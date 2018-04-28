#include "Szczur/Modules/Cinematics/MoviePlayer.hpp"

bool MoviePlayer::loadFromFile(const char * filename)
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

void MoviePlayer::jumpTo(const unsigned int &seekTarget)
{
    if(m_sound)
    {
        for(auto p : m_sound->g_videoPkts)
        {
            av_free_packet(p);
           // av_free(p);
        }
        m_sound->g_videoPkts.clear();
        int64_t seekOnVideo = seekTarget;
        seekOnVideo = av_rescale_q(seekOnVideo, AV_TIME_BASE_Q, m_pFormatCtx->streams[m_videoStream]->time_base);
        auto ret = avformat_seek_file(m_pFormatCtx, m_videoStream, 0, seekOnVideo, seekOnVideo, AVSEEK_FLAG_BACKWARD);
        assert(ret >= 0);
        avcodec_flush_buffers(m_pCodecCtx);
        m_syncAV = true;
    }
}

void MoviePlayer::play()
{
    int64_t duration = m_pFormatCtx->duration;
    const int FrameSize = m_pCodecCtx->width * m_pCodecCtx->height * 3;

     sf::Uint8* Data = new sf::Uint8[m_pCodecCtx->width * m_pCodecCtx->height * 4];
    
    auto &window = getModule<rat::Window>().getWindow(); 

    m_im_video.create(m_pCodecCtx->width, m_pCodecCtx->height);
    m_im_video.setSmooth(false);

    sf::Sprite sprite(m_im_video);

    float x = window.getSize().x;
    float y = window.getSize().y;
    sprite.setScale(x/m_im_video.getSize().x,y/m_im_video.getSize().y);

   
    m_sound = new MovieSound(m_pFormatCtx,m_audioStream);
    m_sound->play();
    m_VClock = new sf::Clock;
    int IdeltaTime;
    int IstartTime;
    m_videoTime=0;
    while (window.isOpen())
    {
        IstartTime = m_VClock->getElapsedTime().asMicroseconds();
        if(m_videoTime>=duration)
        {
            for(auto p : m_sound->g_videoPkts)
            {
                av_free_packet(p);
               // av_free(p);
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
            window.clear();
            window.display();
            delete [] Data;
            return;
        }
       
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                window.close();
            } 
        }
       
        AVPacket* packet_ptr = nullptr;
        
        if(m_sound->g_videoPkts.size() < 150)
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
        
        if(m_sound->timeElapsed() > m_lastDecodedTimeStamp && m_sound->isAudioReady() && !m_sound->g_videoPkts.empty())
        {
            packet_ptr = m_sound->g_videoPkts.front();
            m_sound->g_videoPkts.pop_front();
            
            auto decodedLength = avcodec_decode_video2(m_pCodecCtx, m_pFrame, &m_frameFinished, packet_ptr);
            
            if(m_frameFinished)
            {
                sws_scale(m_sws_ctx, (uint8_t const * const *)m_pFrame->data, m_pFrame->linesize, 0, m_pCodecCtx->height, m_pFrameRGB->data, m_pFrameRGB->linesize);
                
                for (int i = 0, j = 0; i < FrameSize; i += 3, j += 4)
                {
                    Data[j + 0] = m_pFrameRGB->data[0][i + 0];
                    Data[j + 1] = m_pFrameRGB->data[0][i + 1];
                    Data[j + 2] = m_pFrameRGB->data[0][i + 2];
                    Data[j + 3] = 255;
                }
                m_im_video.update(Data);
                
                window.clear();
                
                window.draw(sprite);
                
                window.display();
                
                int64_t timestamp = av_frame_get_best_effort_timestamp(m_pFrame);
                int64_t startTime = pStream->start_time != AV_NOPTS_VALUE ? pStream->start_time : 0;
                int64_t ms = 1000 * (timestamp - startTime) * av_q2d(pStream->time_base);
                m_lastDecodedTimeStamp = ms;
                
                if(m_syncAV)
                {
                    m_blockPts = ms;
                    m_sound->setPlayingOffset(sf::milliseconds(m_blockPts));
                    m_videoTime = m_sound->timeElapsed()*1000;
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
    IdeltaTime = m_VClock->getElapsedTime().asMicroseconds() - IstartTime;
    m_videoTime +=IdeltaTime;
    }
    
    
    sws_freeContext(m_sws_ctx);
    av_free(m_buffer);
    av_free(m_pFrameRGB);
    av_free(m_pFrame);
    avcodec_close(m_pCodecCtx);
    avcodec_close(m_paCodecCtx);
    avformat_close_input(&m_pFormatCtx);
    
    delete [] Data;

    return;
}