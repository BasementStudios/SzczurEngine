#include "StepsManager.hpp"

namespace rat
{
namespace journal
{
    StepsManager::StepsManager(sf::Font *font,Widget* interface)
    :_font(font)
    {
        _scroller = new ScrollAreaWidget;
        _list = new ListWidget;
        interface->add(_scroller);
        _scroller->setPropSize(0.4f, 0.3f);
        _scroller->setPropPosition(0.3f, 0.3f);
        _list->setBetweenPadding(20.f);
        _list->setPropSize(0.1f, 0.f);
        _list->setPropPosition(0.3f, 0.f);
        _scroller->add(_list);
    }

    StepsManager::~StepsManager()
    {

    }

    void StepsManager::refresh()
    {
        _list->clear();
        _steps = std::make_shared<std::vector<std::string> >(_quest->getSteps());
        TextAreaWidget* widget;
        for(auto i = _steps->begin();i!=_steps->end();i++)
        {
            widget = new TextAreaWidget;
            widget->setString(*i);
            widget->setFont(_font);
            widget->setCharacterSize(17);
            widget->setColor(sf::Color(135, 89, 247 ,255));
            widget->setPropSize(0.35f, 0.f);
            _list->add(widget);
        }
        _scroller->resetScrollerPosition();
   
    }
    void StepsManager::setScrollTextures(sf::Texture *t1,sf::Texture *t2,sf::Texture *t3)
    {
        _scroller->setScrollerTexture(t1);
        _scroller->setPathTexture(t2);
        _scroller->setBoundsTexture(t3);
    }

    void StepsManager::setQuest(std::shared_ptr<Quest> quest)
    {
        _list->clear();
        _quest = quest;
        _steps = std::make_shared<std::vector<std::string> >(quest->getSteps());
        TextAreaWidget* widget;
        for(auto i = _steps->begin();i!=_steps->end();i++)
        {
            widget = new TextAreaWidget;
            widget->setString(*i);
            widget->setFont(_font);
            widget->setCharacterSize(17);
            widget->setPropSize(0.35f, 0.f);
            widget->setColor(sf::Color(135, 89, 247 ,255));
            _list->add(widget);
        }
        _scroller->resetScrollerPosition();
    }

    void StepsManager::clear()
    {
        _list->clear();
    }
}
}