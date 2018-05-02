#pragma once
#include "Szczur/Modules/Cinematics/MovieSound.hpp"
#include "Szczur/Modules/Cinematics/VideoLoop.hpp"
#include "Szczur/Application.hpp"

class VideoLoop;

/*
 Main class of playing movies.  
 For this moment class loads .mp4 correctly
 Just call loadFromFile(<filepath>) and play
*/
class MoviePlayer :public rat::Module<rat::Window>
{
public:

    typedef void(*callme)();

    MoviePlayer() = default;
    bool loadFromFile(const char* filename);
    void play();
    void jumpTo(const unsigned int &seekTarget);
    void setFont(sf::Font &font);
    void addLoop(unsigned int startTime,unsigned int endTime,callme fevent1,const char *text1,callme fevent2,const char *text2);
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

    sf::Font m_font;
    std::vector<std::shared_ptr<VideoLoop> > m_loops;
};