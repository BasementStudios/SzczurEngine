#pragma once
#include "Szczur/Modules/Cinematics/MovieSound.hpp"
#include "Szczur/Modules/Cinematics/VideoLoop.hpp"
#include "Szczur/Application.hpp"

class VideoLoop;

/*
 Main class of playing movies.  
 Just call loadFromFile(<filepath>) and play()
*/
class MoviePlayer :public rat::Module<rat::Window>
{
public:
    /* callback */
    typedef void(*callme)();

    MoviePlayer() = default;


/*
    Simple method to load a movie. Type path to file and if everything will be ok function return true.
*/
    bool loadFromFile(const char* filename);


/*
    Nothing special... Just play!
    Ah, and remember that you have to use "loadFromFile" before.
*/
    void play();


/*
    Method for video seeking. Better don't use it if you already used "addLoop"
    "seekTarget" is time expressed with microseconds. 
*/
    void jumpTo(const unsigned int &seekTarget);


/*
    Don't forget to set your font if you create a video loop with "addLoop"
*/
    void setFont(sf::Font &font);

/* 
    Call it if you want add video loop like when main character have to make a decision and video should not play without this decision
    MoviePlayer will show "text1" and "text2" on the screen as sf::Text. When video come to "endtime", everything will return to "startTime"
    During watch a movie we can choose an option and press "enter" button to call one of callback "fevent1" or "fevent2". callback should save decision etc... 
    Sometimes after make a decision you maybe want to skip part of movie and watch other part. "jump1" and "jump2" is a new playback time - movie will skip to this moment
    after press "enter" button. If you type 0 MoviePlayer will not skip anything. Don't forget to set font with "setFont" method. Time is expressed with microseconds.
*/
    void addLoop(unsigned int startTime,unsigned int endTime,callme fevent1,const char *text1,int jump1,callme fevent2,const char *text2,int jump2);

/*
    Text position will be set automatically but you can change it in anytime.
*/
    void setTextOnePosition(int x,int y);
    void setTextTwoPosition(int x,int y);
/*
    Scale like position will be set automatically but if you find text is to big for you or you can't see it because is too small - call this method and design your own scale
*/
    void setTextScale(float x,float y);



private:
    MovieSound          *m_sound = nullptr;
    AVFormatContext     *m_pFormatCtx = nullptr;
    AVCodecContext      *m_pCodecCtx = nullptr;
    AVCodecContext      *m_paCodecCtx = nullptr;
    AVCodec             *m_pCodec = nullptr;
    AVCodec             *m_paCodec = nullptr;
    AVFrame             *m_pFrame = nullptr;
    AVFrame             *m_pFrameRGB = nullptr;
    int                 m_frameFinished;
    int                 m_numBytes;
    uint8_t             *m_buffer = nullptr;
    int                 m_ICurrentFrame = 0;
    int                 ICurrentLoop = 0;
    int                 max = 0;
    float               IframeTime;
    AVDictionary        *m_optionsDict = nullptr;
    AVDictionary        *m_optionsDictA = nullptr;
    SwsContext          *m_sws_ctx = nullptr;
    sf::Texture         m_im_video;
    bool                m_syncAV = false;
    int64_t             m_blockPts = 0;

    sf::Clock           *m_VClock;
    int                 m_videoStream;
    int                 m_audioStream;

    int                 m_jump =0;
    int                 m_lastDecodedTimeStamp=0;
    bool                m_isInit = false;
    sf::Font            m_font;

     std::vector<AVPacket*> m_audioSyncBuffer;
    std::vector<std::shared_ptr<VideoLoop> > m_loops;
  
};