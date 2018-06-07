#include "MovieText.hpp"



namespace rat
{
MovieText::MovieText(callme callback,const char * text,int jump,unsigned int size)
{  
    func = callback;
    m_text.setCharacterSize(size);
    m_text.setString(text);
    m_set = false;
    m_jump = jump;
}

bool MovieText::isSet()
{
    return m_set;
}

void MovieText::set(bool val)
{
    m_set=val;
}

void MovieText::setFont(sf::Font &font)
{
    m_text.setFont(font);
}

void MovieText::setCharacterSize(const unsigned int &size)
{
    m_text.setCharacterSize(size);
}

void MovieText::setSize(const sf::Vector2f &vec)
{
    m_text.setScale(vec);
}

void MovieText::setPosition(const sf::Vector2i &vec)
{
    m_text.setPosition(vec.x,vec.y);
}

void MovieText::draw(sf::RenderTarget& window)
{
    window.draw(m_text);
}

void MovieText::setColor(const sf::Color &color)
{
    m_text.setColor(color);
}

void MovieText::callEvent()
{
    func();
}

int MovieText::getJump()
{
    return m_jump;
}
}