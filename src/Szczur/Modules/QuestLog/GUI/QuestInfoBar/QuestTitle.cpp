#include "QuestTitle.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"

namespace rat
{
    QuestTitle::QuestTitle()
    {
        _title = new TextWidget;
        _addWidget(_title);
        _req = new TextWidget;
        _addWidget(_req);

        _title->setCharacterSize(15);
        _req->setCharacterSize(15);

        _title->setPosition(40, 0);

        _title->setColor(sf::Color::White);
        _req->setColor(sf::Color::White);

    }

    void QuestTitle::setSize(const sf::Vector2u& size)
    {
        BaseBar::setSize(size);
        //_req->setPosition(float(size.x), 0.f);
    }
    void QuestTitle::setSize(unsigned int width, unsigned int height)
    {
        setSize({width, height});
    }

    void QuestTitle::setTitle(const TitleInfo& info)
    {
        _title->setString(info.title);
        if(info.type == TitleInfo::Type::Counter)
        {
            _req->setString(std::to_string(info.counter.current) + "/" + std::to_string(info.counter.maximum));
        }
        else if(info.type == TitleInfo::Type::Req)
        {
            _req->setString(info.req.isSuited ? "[V]" : "[ ]");
        }
        else
        {
            _req->setString("");
        }
    }

    void QuestTitle::setFont(sf::Font* font)
    {
        _font = font;
        _title->setFont(font);
        _req->setFont(font);
    }
}