#include "Szczur/Modules/Cinematics/VideoLoop.hpp"

VideoLoop::VideoLoop(unsigned int startTime,unsigned int endTime,callme fevent1,const char *text1,callme fevent2,const char *text2)
:   m_timeEnd(endTime),
    m_timeStart(startTime),
    m_currentTime(0),
    m_draw(false)
{
    m_reply1 = new LoopText(fevent1,text1);
    m_reply2 = new LoopText(fevent2,text2);
    m_reply1->set(true);
    m_reply2->setColor(sf::Color::Black);
    m_reply1->setColor(sf::Color::White);
}

VideoLoop::~VideoLoop()
{
    if(m_reply1->isSet())
    {
        m_reply1->callEvent();
    }
    else m_reply2->callEvent();
}

void VideoLoop::draw()
{
    if(m_currentTime>=m_timeStart)
    {
        m_draw = true;
    }
    if(m_draw)
    {
        m_reply1->draw();
        m_reply2->draw();
    }
}

void VideoLoop::setDraw(bool val)
{
    m_draw = val;
}

int VideoLoop::getStartTime()
{
    return m_timeStart;
}

int VideoLoop::getEndTime()
{
    return m_timeEnd;
}
void VideoLoop::setFont(sf::Font &font)
{
    m_reply1->setFont(font);
    m_reply2->setFont(font);
}

int VideoLoop::update(const int &deltaTime)
{
    m_currentTime = deltaTime;
    if(m_currentTime<m_timeEnd)
    {
        return -1;
    }
    return m_timeStart;
}

void VideoLoop::change()
{
    if(m_reply1->isSet())
    {
        m_reply1->set(false);
        m_reply2->set(true);
        m_reply1->setColor(sf::Color::Black);
        m_reply2->setColor(sf::Color::White);
    }
    else
    {
        m_reply1->set(true);
        m_reply2->set(false);
        m_reply2->setColor(sf::Color::Black);
        m_reply1->setColor(sf::Color::White);
    }
}

void VideoLoop::init(const sf::Vector2u size)
{
    m_reply1->setPosition(sf::Vector2f(size.x/1.3f,size.y/2.5f));
    m_reply2->setPosition(sf::Vector2f(size.x/1.3f,size.y/2.f));
}

void VideoLoop::setTime(const int time)
{
    m_currentTime = time;
}