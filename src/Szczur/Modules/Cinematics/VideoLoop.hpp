#pragma once
#include "Szczur/Modules/Cinematics/LoopText.hpp"
class LoopText;

class VideoLoop
{
public:

    typedef void(* callme)();
    VideoLoop(unsigned int startTime,unsigned int endTime,callme fevent1,const char *text1,callme fevent2,const char *text2);
    ~VideoLoop();
    void draw();
    int update(const int &deltaTime);
    void setTime(const int time);
    void setFont(sf::Font &font);
    void init(const sf::Vector2u size);
    void change();
    int getStartTime();
    int getEndTime();

    void setDraw(bool val);
private:
    bool m_draw;
    int m_currentTime;
    int m_timeStart;
    int m_timeEnd;
    LoopText *m_reply1;
    LoopText *m_reply2;
};