#pragma once
#include <SFML/Graphics.hpp>
#include "Szczur/Application.hpp"
class LoopText
    : public rat::Module<rat::Window>
{
    public:
        typedef void(*callme)();

        LoopText(callme callback,const char * text,int jump,unsigned int size = 50u);
        
        ~LoopText();

        bool isSet();

        void set(bool val);
        
        void setFont(sf::Font &font);

        void setCharacterSize(const unsigned int &size);

        void setSize(const sf::Vector2f &vec);

        void setPosition(const sf::Vector2i &vec);

        void draw();

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