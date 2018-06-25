#pragma once

#include <functional>

namespace sf {
    class RenderTarget;
    class Font;
}
#include <SFML/System/Vector2.hpp>

#include "MovieText.hpp"

namespace rat
{

class VideoLoop
{
public:
    
    VideoLoop(
        unsigned int startTime, unsigned int endTime, 
        std::function<void()> fevent1, const char* text1, int jump1,
        std::function<void()> fevent2, const char* text2, int jump2
    );
    ~VideoLoop();
    
    void draw(sf::RenderTarget& window);
    int update(const int &deltaTime);
    void setTime(const int time);
    void setFont(sf::Font &font);
    void init(const sf::Vector2u size);
    int change();
    int getStartTime();
    int getEndTime();
    void setScale(sf::Vector2f scale);

    void setDraw(bool val);
    void setPositionOne(sf::Vector2i pos);
    void setPositionTwo(sf::Vector2i pos);
    int startJump;
private:
    bool m_draw;
    int m_currentTime;
    int m_timeStart;
    int m_timeEnd;
    MovieText *m_reply1;
    MovieText *m_reply2;
};
}