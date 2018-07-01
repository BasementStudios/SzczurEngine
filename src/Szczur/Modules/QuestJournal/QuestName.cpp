#include "QuestName.hpp"

namespace rat
{
namespace journal
{
    QuestName::QuestName(sf::Font *font,InterfaceWidget *interface)
    :_font(font)
    {
        _widget = new TextWidget;
       // _widget->setPropPosition(0.60f,0.1);
       _widget->setPropPosition(0.63f,0.06f);
        _widget->setColor(sf::Color(135, 89, 247 ,255));
        _widget->setFont(_font);
        interface->add(_widget);
    }

    QuestName::~QuestName()
    {

    }

    void QuestName::setQuest(std::shared_ptr<Quest> quest)
    {
        _quest = quest;
        _questName = _quest->getQuestName();
        _widget->setString(_questName);
    }

    void QuestName::clear()
    {
        _widget->setString("");   
    }
}
}