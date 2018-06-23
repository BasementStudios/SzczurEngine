#include "DescriptionManager.hpp"
namespace rat
{
    DescriptionManager::DescriptionManager(sf::Font *font,InterfaceWidget* interface)
    :_font(font)
    {
        interface->add(&_scroller);
        _scroller.setPropSize(0.7f, 0.83f);
        _scroller.setPropPosition(0.94f, 0.85f);
        _list.setBetweenPadding(20.f);
        _list.setPropSize(0.3f, 1.f);

        _scroller.add(&_list);
        _scroller.setPosition(20,20);
    }

    DescriptionManager::~DescriptionManager()
    {

    }

    void DescriptionManager::refresh()
    {
        _descriptions = std::make_shared<std::vector<std::string> >(_quest->getDescription());
        TextWidget* widget;
        for(auto i = _descriptions->begin();i!=_descriptions->end();i++)
        {
            widget = new TextWidget;
            widget->setString(*i);
            widget->setFont(_font);
            widget->setCharacterSize(20);
            widget->setColor(sf::Color::White);
            _list.add(widget);
            LOG_INFO(widget->getLength());
        }
    }

    void DescriptionManager::setQuest(std::shared_ptr<Quest> quest)
    {
        _quest = quest;
        _descriptions = std::make_shared<std::vector<std::string> >(quest->getDescription());
        TextWidget* widget;
        for(auto i = _descriptions->begin();i!=_descriptions->end();i++)
        {
            widget = new TextWidget;
            widget->setString(*i);
            widget->setFont(_font);
            widget->setCharacterSize(20);
            widget->setColor(sf::Color::White);
            _list.add(widget);
        }
    }

    void DescriptionManager::setScrollTextures(sf::Texture* t1,sf::Texture* t2,sf::Texture* t3)
    {
        _scroller.setScrollerTexture(t1);
        _scroller.setPathTexture(t2);
        _scroller.setBoundsTexture(t3);
    }
}