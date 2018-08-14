#pragma once
#include <functional>

#include <Szczur/Modules/Window/Window.hpp>
#include <Szczur/Modules/Script/Script.hpp>
#include <Szczur/Modules/Input/Input.hpp>
#include <Szczur/Utility/Logger.hpp>

#include "MovieSound.hpp"
#include "VideoLoop.hpp"

/*
 Main class of playing movies.
 Just call loadFromFile(<filepath>) and play()
*/
namespace rat
{

class Cinematics :public Module<Input,Window,Script>
{
public:
    /* callback */
    typedef sol::function callme;

    Cinematics();
    ~Cinematics();

    // Disable coping
	Cinematics(const Cinematics&) = delete;
	Cinematics& operator = (const Cinematics&) = delete;

	// Disable moving
	Cinematics(Cinematics&&) = delete;
	Cinematics& operator = (Cinematics&&) = delete;
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
    "seekTarget" is a time expressed with microseconds.
*/
    void jumpTo(const unsigned int &seekTarget);


/*
    Don't forget to set your font if you create a video loop with "addLoop"
*/
    void setFont(sf::Font &font);
    void setFontPath(const char *filename);


/*
    Call it if you want add video loop like when main character have to make a decision and video should not play without this decision
    Cinematics will show "text1" and "text2" on the screen as sf::Text. When video come to "endtime", everything will return to "startTime"
    During watch a movie we can choose an option and press "enter" button to call one of callback "fevent1" or "fevent2". callback should save decision etc...
    Sometimes after make a decision you maybe want to skip part of movie and watch other part. "jump1" and "jump2" is a new playback time - movie will skip to this moment
    after press "enter" button. If you type 0 Cinematics will not skip anything. Don't forget to set font with "setFont" method. Time is expressed with microseconds.
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

/*
    Drawing current frame
*/
    void render();
/*
    We should not try drawing frame without decode next packets,
    that's what "update" is doing - dirty job ^^
*/
    void update();


/*
    Wrong! wrong! wrong! Absolutely stop everything!
*/
    void skip();

/*
    If you want to reduce the volume call this function
*/
    void setVolume(float vol);

/*
    Ask me if this movie still playing
*/
    bool isPlaying();

/*
    Tell me what I should do at the end of the movie
*/
    void setCallbackFinish(callme t);

private:
    void endVideo();

    void initScript();
    MovieSound          *_sound;

    AVFormatContext     *_pFormatCtx;
    AVCodecContext      *_pCodecCtx;
    AVCodecContext      *_paCodecCtx;
    AVCodec             *_pCodec;
    AVCodec             *_paCodec;
    AVFrame             *_pFrame;
    AVFrame             *_pFrameRGB;
    AVDictionary        *_optionsDict;
    AVDictionary        *_optionsDictA;
    SwsContext          *_sws_ctx;

    uint8_t             *_buffer;
    size_t              _count;
    int64_t             _duration;
    int64_t             _blockPts = 0;

    bool                _syncAV = false;
    bool                _isInit = false;
    bool                _play = false;
    bool                _isMusic = false;
    bool                _isCallbackSet = false;

    int                 _ICurrentLoop;
    int                 _frameFinished;
    int                 _numBytes;
    int                 _videoStream;
    int                 _audioStream;
    int                 _ISmax;
    int                 _jump =0;
    int                 _lastDecodedTimeStamp=0;
    int                 _IdeltaTime;
    int                 _IstartTime;
    int                 _FrameSize;
    unsigned int        _alfa;

    callme              _callbackFinish;

    sf::Uint8*          _data;
    sf::Sprite          _sprite;
    sf::Font            _font;
    sf::Texture         _im_video;
    sf::Clock           *_VClock;
    sf::RenderTexture   canvas;

    std::vector<MyPacket*> _audioSyncBuffer;
    std::vector<std::shared_ptr<VideoLoop> > _loops;

};
}
