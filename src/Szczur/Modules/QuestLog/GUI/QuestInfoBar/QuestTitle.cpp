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

        _title->setColor(sf::Color::White);
        _req->setColor(sf::Color::White);

        _req->setPropOrigin(1.f, 0.f);
    }

    void QuestTitle::setSize(const sf::Vector2u& size)
    {
        BaseBar::setSize(size);
        _req->setPosition(float(size.x), 0.f);
    }
    void QuestTitle::setSize(unsigned int width, unsigned int height)
    {
        setSize({width, height});
    }

    void QuestTitle::setReq(const std::string& title)
    {
        _title->setString(title);
        _req->setString("");
    }
    void QuestTitle::setReq(const std::string& title, int current, int maximum)
    {
        setReq(title);
        _req->setString(std::to_string(current) + "/" + std::to_string(maximum));
    }
    void QuestTitle::setReq(const std::string& title, bool req)
    {
        setReq(title);
        _req->setString(req ? "[V]" : "[ ]");
    }

    void QuestTitle::setFont(sf::Font* font)
    {
        _font = font;
        _title->setFont(font);
        _req->setFont(font);
    }
}