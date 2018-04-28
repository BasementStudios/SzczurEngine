#pragma once
#include "Szczur/Modules/Cinematics/MovieSound.hpp"
#include "Szczur/Application.hpp"
/*
 Main class of playing movies.  
 For this moment class loads .mp4 correctly
 Just call loadFromFile(<filepath>) and play
*/
class MoviePlayer :public rat::Module<rat::Window>
{
public:
    MoviePlayer() = default;
    bool loadFromFile(const char* filename);
    void play();
    void jumpTo(const unsigned int &seekTarget);
private:
    MovieSound *m_sound = nullptr;
    AVFormatContext *m_pFormatCtx = nullptr;
    AVCodecContext  *m_pCodecCtx = nullptr;
    AVCodecContext  *m_paCodecCtx = nullptr;
    AVCodec         *m_pCodec = nullptr;
    AVCodec         *m_paCodec = nullptr;
    AVFrame         *m_pFrame = nullptr;
    AVFrame         *m_pFrameRGB = nullptr;
    int             m_frameFinished;
    int             m_numBytes;
    uint8_t         *m_buffer = nullptr;
    int64_t         m_lastDecodedTimeStamp = 0;
    
    AVDictionary    *m_optionsDict = nullptr;
    AVDictionary    *m_optionsDictA = nullptr;
    SwsContext      *m_sws_ctx = nullptr;
    sf::Texture m_im_video;
    bool m_syncAV = false;
    int64_t m_blockPts = 0;
    std::vector<AVPacket*> m_audioSyncBuffer;

    sf::Clock *m_VClock;
    int m_videoStream;
    int m_audioStream;
    int m_videoTime;

};