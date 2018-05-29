#include "QuestGUI.hpp"

namespace rat
{
    QuestGUI::QuestGUI()
    {
        _title.setSize({100u, 25u});
        _title.setReq("None");
        _addBar(_title);
    }
    //void QuestGUI::setQuestName(const std::string& name);
    void QuestGUI::resetSubtitles()
    {
        for(auto& sub : _subTitles)
        {
            sub->deactivate();
        }
        _activeAmount = 0;
    }
    void QuestGUI::setTitle(const std::string& name)
    {
        _title.setReq(name);
    }
    
    QuestTitle* QuestGUI::addSubtitle(const std::string& name)
    {
        auto* sub = _getSubTitle(_activeAmount++);

        sub->setReq(name);
        return sub;
    }
    QuestTitle* QuestGUI::addSubtitle(const std::string& name, int current, int maximum)
    {
        auto* sub = _getSubTitle(_activeAmount++);

        sub->setReq(name, current, maximum);
        return sub;
    }
    QuestTitle* QuestGUI::addSubtitle(const std::string& name, bool isSuited)
    {
        auto* sub = _getSubTitle(_activeAmount++);

        sub->setReq(name, isSuited);
        return sub;
    }
    //void QuestGUI::popSubtitle();

    QuestTitle* QuestGUI::_getSubTitle(size_t index)
    {
        if(index >= _madeAmount)
        {
            _addBar(*_subTitles.emplace_back(std::make_unique<QuestTitle>()));
            _madeAmount++;
        }
        auto& sub = _subTitles[index];
        sub->activate();
        sub->setSize({100u, 25u});
        sub->setPosition(_titlePadding, (25.f + _titlePadding) * float(index + 1));
        sub->setFont(_font);
        
        return sub.get();
    }

    void QuestGUI::setFont(sf::Font* font)
    {
        _font = font;
        _title.setFont(font);
    }

}
