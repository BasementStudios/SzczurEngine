#include "StepsManager.hpp"

namespace rat
{
    StepsManager::StepsManager(sf::Font *font,InterfaceWidget* interface)
    :_font(font)
    {
        interface->add(&_scroller);
        _scroller.setPropSize(0.4f, 0.3f);
        _scroller.setPropPosition(0.1f, 0.1f);
        _list.setBetweenPadding(20.f);
        _list.setPropSize(0.3f, 1.f);

        _scroller.add(&_list);
    }

    StepsManager::~StepsManager()
    {

    }

    void StepsManager::refresh()
    {
        _list.clear();
        _steps = std::make_shared<std::vector<std::string> >(_quest->getSteps());
        TextWidget* widget;
        for(auto i = _steps->begin();i!=_steps->end();i++)
        {
            widget = new TextWidget;
            widget->setString(*i);
            widget->setFont(_font);
            widget->setCharacterSize(25);
            widget->setColor(sf::Color::White);
            _list.add(widget);
        }
   
    }
    void StepsManager::setScrollTextures(sf::Texture *t1,sf::Texture *t2,sf::Texture *t3)
    {
        _scroller.setScrollerTexture(t1);
        _scroller.setPathTexture(t2);
        _scroller.setBoundsTexture(t3);
    }

    void StepsManager::setQuest(std::shared_ptr<Quest> quest)
    {
        _list.clear();
        _quest = quest;
        _steps = std::make_shared<std::vector<std::string> >(quest->getSteps());
        TextWidget* widget;
        for(auto i = _steps->begin();i!=_steps->end();i++)
        {
            widget = new TextWidget;
            widget->setString(*i);
            widget->setFont(_font);
            widget->setCharacterSize(25);
            widget->setColor(sf::Color::White);
            _list.add(widget);
        }
    }

}