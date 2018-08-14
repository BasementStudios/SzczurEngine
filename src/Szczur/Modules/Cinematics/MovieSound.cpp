#include "MovieSound.hpp"

namespace rat
{
MovieSound::MovieSound(AVFormatContext* ctx, int index)
: _formatCtx(ctx)
, _audioStreamIndex(index)
, _codecCtx(ctx->streams[index]->codec)
{
    _audioFrame = av_frame_alloc();
    
    _sampleRate = _codecCtx->sample_rate;
    _samplesBuffer = (sf::Int16*)av_malloc(sizeof(sf::Int16) * av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO) * _sampleRate * 2);
    
    initialize(av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO), _sampleRate);
    
    initResampler();
    
    initialTime = sf::SoundStream::getPlayingOffset();
    
}

MovieSound::~MovieSound()
{
    
}

void MovieSound::initResampler()
{
    int err = 0;
    _swrCtx = swr_alloc();
    
    if(_codecCtx->channel_layout == 0)
    {
        _codecCtx->channel_layout = av_get_default_channel_layout(_codecCtx->channels);
    }
    
    /* set options */
    av_opt_set_int(_swrCtx, "in_channel_layout",    _codecCtx->channel_layout, 0);
    av_opt_set_int(_swrCtx, "in_sample_rate",       _codecCtx->sample_rate, 0);
    av_opt_set_sample_fmt(_swrCtx, "in_sample_fmt", _codecCtx->sample_fmt, 0);
    av_opt_set_int(_swrCtx, "out_channel_layout",    AV_CH_LAYOUT_STEREO, 0);
    av_opt_set_int(_swrCtx, "out_sample_rate",       _codecCtx->sample_rate, 0);
    av_opt_set_sample_fmt(_swrCtx, "out_sample_fmt", AV_SAMPLE_FMT_S16, 0);
    
    err = swr_init(_swrCtx);
    
    _maxDstNbSamples = _dstNbSamples = 1024;
    
    _dstNbChannels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
    err = av_samples_alloc_array_and_samples(&_dstData, &_dstLinesize, _dstNbChannels, _dstNbSamples, AV_SAMPLE_FMT_S16, 0);
}
bool MovieSound::decodePacket(MyPacket* packet, AVFrame* outputFrame, bool& gotFrame)
{
    if(_play)
    {
        bool needsMoreDecoding = false;
        int igotFrame = 0;
        
        int decodedLength = avcodec_decode_audio4(_codecCtx, outputFrame, &igotFrame, packet);
        gotFrame = (igotFrame != 0);
        
        if(decodedLength < packet->size)
        {
            needsMoreDecoding = true;
            packet->data += decodedLength;
            packet->size -= decodedLength;
        }
        
        return needsMoreDecoding;
    }
    else return false;
}

void MovieSound::resampleFrame(AVFrame *frame, uint8_t *&outSamples, int &outNbSamples, int &outSamplesLength)
{
    int err = 0;
    int src_rate = frame->sample_rate;
    int dst_rate = frame->sample_rate;
    
    _dstNbSamples = av_rescale_rnd(swr_get_delay(_swrCtx, src_rate) + frame->nb_samples, dst_rate, src_rate, AV_ROUND_UP);
    
    if(_dstNbSamples > _maxDstNbSamples)
    {
        av_free(_dstData[0]);
        err = av_samples_alloc(_dstData, &_dstLinesize, _dstNbChannels, _dstNbSamples, AV_SAMPLE_FMT_S16, 1);
        _maxDstNbSamples = _dstNbSamples;
    }
    
    err = swr_convert(_swrCtx, _dstData, _dstNbSamples, (const uint8_t**)frame->extended_data, frame->nb_samples);
    
    int dst_bufsize = av_samples_get_buffer_size(&_dstLinesize, _dstNbChannels, err, AV_SAMPLE_FMT_S16, 1);
    
    outNbSamples = dst_bufsize / av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
    outSamplesLength = dst_bufsize;
    outSamples = _dstData[0];
}

bool MovieSound::onGetData(sf::SoundStream::Chunk &data)
{
    data.samples = _samplesBuffer;
    std::deque<MyPacket*> *pack;
    pack = &g_audioPkts;

    auto func = [pack]  {
            return !pack->empty();
        };
     
    while (data.sampleCount < av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO) * _sampleRate)
    {
        if(!_play) return false;
        bool needsMoreDecoding = false;
        bool gotFrame = false;
        
       std::unique_lock<std::mutex> lk(g_mut);

        g_newPktCondition.wait(lk,func);

        MyPacket* packet = g_audioPkts.front();
        
        g_audioPkts.pop_front();
      
        do {
            needsMoreDecoding = decodePacket(packet, _audioFrame, gotFrame);     
             
            if (gotFrame)
            {
                uint8_t* samples = NULL;
                int nbSamples = 0;
                int samplesLength = 0;
                
                resampleFrame(_audioFrame, samples, nbSamples, samplesLength);
                
                std::memcpy((void*)(data.samples + data.sampleCount), samples, samplesLength);
                data.sampleCount += nbSamples;
            }
            
        }while (needsMoreDecoding);
        
        lk.unlock();
        
        //av_free_packet(packet);
        //av_free(packet);
        packet->freePacket();
        packet->free();
    }
    return true;
}

void MovieSound::onSeek(sf::Time timeOffset)
{
    std::lock_guard<std::mutex> lk(g_mut);
    for (auto p : g_audioPkts)
    {
        p->freePacket();
        p->free();
    }
    g_audioPkts.clear();
    avcodec_flush_buffers(_codecCtx);
}

void MovieSound::stop()
{
    _play = false;
   //sf::SoundStream::stop(); <---- crash
}
}