#include "Szczur/Modules/Cinematics/MoviePlayer.hpp"


bool MoviePlayer::loadFromFile(const char * filename)
{
    av_register_all();

    if(avformat_open_input(&pFormatCtx, filename, NULL, NULL)!=0)
        return false; 
    
    if(avformat_find_stream_info(pFormatCtx, NULL)<0)
        return false; 
    
    
    av_dump_format(pFormatCtx, 0, filename, 0);
    
    videoStream = -1;
    audioStream = -1;
    for(int i = 0; i < pFormatCtx->nb_streams; ++i)
    {
        if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoStream = i;
        }
        else if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            audioStream = i;
        }
    }
    
    if(videoStream < 0)
        return false;
    
    if(videoStream >= 0)
    {
        pCodecCtx = pFormatCtx->streams[videoStream]->codec;
        pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
        
        if(avcodec_open2(pCodecCtx, pCodec, &optionsDict)<0)
            return false;
    }
    if(audioStream >= 0)
    {
        paCodecCtx = pFormatCtx->streams[audioStream]->codec;
        paCodec = avcodec_find_decoder(paCodecCtx->codec_id);
        
        if(avcodec_open2(paCodecCtx, paCodec, &optionsDictA))
            return false;
    }
 
    pFrame = av_frame_alloc();
    pFrameRGB = av_frame_alloc();
    if (pFrameRGB == nullptr)
    {
        return false;
    }
    
    numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);
    buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));
    
    sws_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);
    
    avpicture_fill((AVPicture*)pFrameRGB, buffer, AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);
}


void MoviePlayer::play()
{

    const int FrameSize = pCodecCtx->width * pCodecCtx->height * 3;

     sf::Uint8* Data = new sf::Uint8[pCodecCtx->width * pCodecCtx->height * 4];
    
    /* example window for drawing movie */
     sf::RenderWindow window(sf::VideoMode(pCodecCtx->width, pCodecCtx->height), "SFML window");

    im_video.create(pCodecCtx->width, pCodecCtx->height);
    im_video.setSmooth(false);


    sf::Sprite sprite(im_video);

    MovieSound sound(pFormatCtx, audioStream);

    sound.play();

    while (window.isOpen())
    {
     
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
        
        AVPacket* packet_ptr = 0;
        
        if(sound.g_videoPkts.size() < 150)
        {
            packet_ptr = (AVPacket*)av_malloc(sizeof(AVPacket));
            av_init_packet(packet_ptr);
            
            if(av_read_frame(pFormatCtx, packet_ptr) < 0)
            {
                if(sound.g_videoPkts.empty() || sound.g_audioPkts.empty())
                {
                    for(auto p : sound.g_videoPkts)
                    {
                        av_free_packet(p);
                        av_free(p);
                    }
                    
                    for(auto p : sound.g_audioPkts)
                    {
                        av_free_packet(p);
                        av_free(p);
                    }
                    
                    break;
                }
                else
                {
                    av_free_packet(packet_ptr);
                    av_free(packet_ptr);
                    
                    packet_ptr = 0;
                }
            }
            
            if(packet_ptr)
            {
                AVPacket& packet = *packet_ptr;
                if(packet.stream_index == videoStream)
                {
                    sound.g_videoPkts.push_back(packet_ptr);
                }
                else if(packet.stream_index == audioStream)
                {
                    if(packet_ptr->pts >= blockPts && !syncAV)
                    {
                        std::lock_guard<std::mutex> lk(sound.g_mut);
                        for(auto p : audioSyncBuffer)
                        {
                            if(p->pts >= blockPts)
                            {
                                sound.g_audioPkts.push_back(p);
                            }
                            else
                            {
                                av_free_packet(p);
                                av_free(p);
                            }
                        }
                        sound.g_audioPkts.push_back(packet_ptr);
                        sound.g_newPktCondition.notify_one();
                        
                        audioSyncBuffer.clear();
                    }
                    
                    if(syncAV)
                    {
                        audioSyncBuffer.push_back(packet_ptr);
                    }
                    
                }
            }
        }
        
        const auto pStream = pFormatCtx->streams[videoStream];
        
        if(sound.timeElapsed() > m_lastDecodedTimeStamp && sound.isAudioReady() && !sound.g_videoPkts.empty())
        {
            packet_ptr = sound.g_videoPkts.front();
            sound.g_videoPkts.pop_front();
            
            auto decodedLength = avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, packet_ptr);
            
            if(frameFinished)
            {
                sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);
                
                for (int i = 0, j = 0; i < FrameSize; i += 3, j += 4)
                {
                    Data[j + 0] = pFrameRGB->data[0][i + 0];
                    Data[j + 1] = pFrameRGB->data[0][i + 1];
                    Data[j + 2] = pFrameRGB->data[0][i + 2];
                    Data[j + 3] = 255;
                }
                
                im_video.update(Data);
                
                window.clear();
                
                window.draw(sprite);
                
                window.display();
                
                int64_t timestamp = av_frame_get_best_effort_timestamp(pFrame);
                int64_t startTime = pStream->start_time != AV_NOPTS_VALUE ? pStream->start_time : 0;
                int64_t ms = 1000 * (timestamp - startTime) * av_q2d(pStream->time_base);
                m_lastDecodedTimeStamp = ms;
                
                if(syncAV)
                {
                    blockPts = ms;
                    sound.setPlayingOffset(sf::milliseconds(blockPts));
                    
                    syncAV = false;
                }
                
            }
            
            if(decodedLength < packet_ptr->size)
            {
                packet_ptr->data += decodedLength;
                packet_ptr->size -= decodedLength;
                
                sound.g_videoPkts.push_front(packet_ptr);
            }
            else
            {
                av_free_packet(packet_ptr);
                av_free(packet_ptr);
            }
        }
        
    }
    
    
    sws_freeContext(sws_ctx);
    av_free(buffer);
    av_free(pFrameRGB);
    av_free(pFrame);
    avcodec_close(pCodecCtx);
    avcodec_close(paCodecCtx);
    avformat_close_input(&pFormatCtx);
    
    delete [] Data;

    return;
}