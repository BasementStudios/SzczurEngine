#include "MovieSound.hpp"

namespace rat
{
MovieSound::MovieSound(AVFormatContext* ctx, int index)
: m_formatCtx(ctx)
, m_audioStreamIndex(index)
, m_codecCtx(ctx->streams[index]->codec)
{
    m_audioFrame = av_frame_alloc();
    
    m_sampleRate = m_codecCtx->sample_rate;
    m_samplesBuffer = (sf::Int16*)av_malloc(sizeof(sf::Int16) * av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO) * m_sampleRate * 2);
    
    initialize(av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO), m_sampleRate);
    
    initResampler();
    
    initialTime = sf::SoundStream::getPlayingOffset();
    
}

MovieSound::~MovieSound()
{
    
}

void MovieSound::initResampler()
{
    int err = 0;
    m_swrCtx = swr_alloc();
    
    if(m_codecCtx->channel_layout == 0)
    {
        m_codecCtx->channel_layout = av_get_default_channel_layout(m_codecCtx->channels);
    }
    
    /* set options */
    av_opt_set_int(m_swrCtx, "in_channel_layout",    m_codecCtx->channel_layout, 0);
    av_opt_set_int(m_swrCtx, "in_sample_rate",       m_codecCtx->sample_rate, 0);
    av_opt_set_sample_fmt(m_swrCtx, "in_sample_fmt", m_codecCtx->sample_fmt, 0);
    av_opt_set_int(m_swrCtx, "out_channel_layout",    AV_CH_LAYOUT_STEREO, 0);
    av_opt_set_int(m_swrCtx, "out_sample_rate",       m_codecCtx->sample_rate, 0);
    av_opt_set_sample_fmt(m_swrCtx, "out_sample_fmt", AV_SAMPLE_FMT_S16, 0);
    
    err = swr_init(m_swrCtx);
    
    m_maxDstNbSamples = m_dstNbSamples = 1024;
    
    m_dstNbChannels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
    err = av_samples_alloc_array_and_samples(&m_dstData, &m_dstLinesize, m_dstNbChannels, m_dstNbSamples, AV_SAMPLE_FMT_S16, 0);
}
bool MovieSound::decodePacket(AVPacket* packet, AVFrame* outputFrame, bool& gotFrame)
{
    bool needsMoreDecoding = false;
    int igotFrame = 0;
    
    int decodedLength = avcodec_decode_audio4(m_codecCtx, outputFrame, &igotFrame, packet);
    gotFrame = (igotFrame != 0);
    
    if(decodedLength < packet->size)
    {
        needsMoreDecoding = true;
        packet->data += decodedLength;
        packet->size -= decodedLength;
    }
    
    return needsMoreDecoding;
}

void MovieSound::resampleFrame(AVFrame *frame, uint8_t *&outSamples, int &outNbSamples, int &outSamplesLength)
{
    int err = 0;
    int src_rate = frame->sample_rate;
    int dst_rate = frame->sample_rate;
    
    m_dstNbSamples = av_rescale_rnd(swr_get_delay(m_swrCtx, src_rate) + frame->nb_samples, dst_rate, src_rate, AV_ROUND_UP);
    
    if(m_dstNbSamples > m_maxDstNbSamples)
    {
        av_free(m_dstData[0]);
        err = av_samples_alloc(m_dstData, &m_dstLinesize, m_dstNbChannels, m_dstNbSamples, AV_SAMPLE_FMT_S16, 1);
        m_maxDstNbSamples = m_dstNbSamples;
    }
    
    err = swr_convert(m_swrCtx, m_dstData, m_dstNbSamples, (const uint8_t**)frame->extended_data, frame->nb_samples);
    
    int dst_bufsize = av_samples_get_buffer_size(&m_dstLinesize, m_dstNbChannels, err, AV_SAMPLE_FMT_S16, 1);
    
    outNbSamples = dst_bufsize / av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
    outSamplesLength = dst_bufsize;
    outSamples = m_dstData[0];
}

bool MovieSound::onGetData(sf::SoundStream::Chunk &data)
{
    data.samples = m_samplesBuffer;
    std::deque<AVPacket*> *pack;
    pack = &g_audioPkts;

    auto func = [pack]  {
            return !pack->empty();
        };
     
    while (data.sampleCount < av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO) * m_sampleRate)
    {
        
        bool needsMoreDecoding = false;
        bool gotFrame = false;
        
       std::unique_lock<std::mutex> lk(g_mut);

        g_newPktCondition.wait(lk,func);

        AVPacket* packet = g_audioPkts.front();
        
        g_audioPkts.pop_front();
      
        do {
            needsMoreDecoding = decodePacket(packet, m_audioFrame, gotFrame);     
             
            if (gotFrame)
            {
                uint8_t* samples = NULL;
                int nbSamples = 0;
                int samplesLength = 0;
                
                resampleFrame(m_audioFrame, samples, nbSamples, samplesLength);
                
                std::memcpy((void*)(data.samples + data.sampleCount), samples, samplesLength);
                data.sampleCount += nbSamples;
            }
            
        }while (needsMoreDecoding);
        
        lk.unlock();
        
        av_free_packet(packet);
        av_free(packet);
    }
    return true;
}

void MovieSound::onSeek(sf::Time timeOffset)
{
    std::lock_guard<std::mutex> lk(g_mut);
    for (auto p : g_audioPkts)
    {
        av_free_packet(p);
        av_free(p);
    }
    g_audioPkts.clear();
    avcodec_flush_buffers(m_codecCtx);
}
}