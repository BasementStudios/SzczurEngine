#pragma once
#include <functional>

#include <Szczur/Modules/Window/Window.hpp>

#include <SFML/Graphics.hpp>

namespace rat
{
class MovieText
{
    public:
        //typedef void(*callme)();
         //using callme = std::function<void()>;
         typedef std::function<void()> callme;

        MovieText(callme callback,const char * text,int jump,unsigned int size = 50u);
        
        ~MovieText();

        bool isSet();

        void set(bool val);
        
        void setFont(sf::Font &font);

        void setCharacterSize(const unsigned int &size);

        void setSize(const sf::Vector2f &vec);

        void setPosition(const sf::Vector2i &vec);

        void draw(sf::RenderTarget& window);

        void setColor(const sf::Color &color);

        int getJump();

        void callEvent();
    private:
        callme func;
        unsigned int m_timeStart;
        unsigned int m_timeEnd;
        sf::Text m_text;
        bool m_set;
        int m_jump;
};
}