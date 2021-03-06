#include "VideoLoop.hpp"

namespace rat
{
VideoLoop::VideoLoop(
    unsigned int startTime, unsigned int endTime, 
    std::function<void()> fevent1, const char* text1, int jump1,
    std::function<void()> fevent2, const char* text2, int jump2
) :
    m_draw(false), 
    m_currentTime(0), 
    m_timeStart(startTime), 
    m_timeEnd(endTime)
{
    m_reply1 = new MovieText(fevent1, text1, jump1);
    m_reply2 = new MovieText(fevent2, text2, jump2);
    m_reply1->set(true);
    startJump = jump1;
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

void VideoLoop::draw(sf::RenderTarget& window)
{
    if(m_currentTime>=m_timeStart)
    {
        m_draw = true;
    }
    if(m_draw)
    {
        m_reply1->draw(window);
        m_reply2->draw(window);
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

int VideoLoop::change()
{
    if(m_reply1->isSet())
    {
        m_reply1->set(false);
        m_reply2->set(true);
        m_reply1->setColor(sf::Color::Black);
        m_reply2->setColor(sf::Color::White);
        return m_reply2->getJump();
    }
    else
    {
        m_reply1->set(true);
        m_reply2->set(false);
        m_reply2->setColor(sf::Color::Black);
        m_reply1->setColor(sf::Color::White);
        return m_reply1->getJump();
    }
}

void VideoLoop::init(const sf::Vector2u size)
{
    m_reply1->setPosition(sf::Vector2i(size.x/1.3f,size.y/2.5f));
    m_reply2->setPosition(sf::Vector2i(size.x/1.3f,size.y/2.f));
}

void VideoLoop::setTime(const int time)
{
    m_currentTime = time;
}

void VideoLoop::setPositionOne(sf::Vector2i pos)
{
    m_reply1->setPosition(pos);
}

void VideoLoop::setPositionTwo(sf::Vector2i pos)
{
    m_reply2->setPosition(pos);
}

void VideoLoop::setScale(sf::Vector2f scale)
{
    m_reply1->setSize(scale);
    m_reply2->setSize(scale);
}
}