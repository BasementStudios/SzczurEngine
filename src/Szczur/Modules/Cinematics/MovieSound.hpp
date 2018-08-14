#pragma once

#include <cstring>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <chrono>
#include <algorithm>
#include <assert.h>
#include <memory>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
#include <libavutil/samplefmt.h>
#include <libavutil/channel_layout.h>
}

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

namespace rat
{

class MyPacket : public AVPacket
{
    public:

        bool _deleted = false;
        bool _deletedPacket = false;

        void free()
        {
            if (!_deleted)
            {
                av_free(this);
                _deleted = true;
            }
        }

        void freePacket()
        {
            if (!_deletedPacket)
            {
                av_free_packet(this);
                _deletedPacket = true;
            }
        }
        

};



class MovieSound : public sf::SoundStream
{
public:

	std::mutex  g_mut;
	std::condition_variable g_newPktCondition;
	std::deque<MyPacket*> g_audioPkts;
	std::deque<MyPacket*> g_videoPkts;

    MovieSound(AVFormatContext* ctx, int index);
    MovieSound() = default;
    virtual ~MovieSound();
    
    bool isAudioReady() const
    {
        return sf::SoundStream::getPlayingOffset() != initialTime;
    }
    
    sf::Int32 timeElapsed() const
    {
        return sf::SoundStream::getPlayingOffset().asMilliseconds();
    }

    void stop();

private:
    
    virtual bool onGetData(Chunk& data);
    virtual void onSeek(sf::Time timeOffset);
    
    bool decodePacket(MyPacket* packet, AVFrame* outputFrame, bool& gotFrame);
    void initResampler();
    void resampleFrame(AVFrame* frame, uint8_t*& outSamples, int& outNbSamples, int& outSamplesLength);
    
    AVFormatContext* _formatCtx;
    AVCodecContext* _codecCtx;
    int _audioStreamIndex;
    
    unsigned _sampleRate;
    sf::Int16* _samplesBuffer;
    AVFrame* _audioFrame;
    
    SwrContext* _swrCtx;
    int _dstNbSamples;
    int _maxDstNbSamples;
    int _dstNbChannels;
    int _dstLinesize;
    uint8_t** _dstData;

    bool _play = true;
    
    sf::Time initialTime;
};
}