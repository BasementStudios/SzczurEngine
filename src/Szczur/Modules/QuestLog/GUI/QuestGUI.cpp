#include "QuestGUI.hpp"

namespace rat
{
    QuestGUI::QuestGUI()
    {
        _title.setSize({200u, 25u});
        TitleInfo info;
        info.makeTitle("None");
        _title.setTitle(info);
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
    void QuestGUI::setTitle(const TitleInfo& info)
    {
        _title.setTitle(info);
    }
    
    QuestTitle* QuestGUI::addSubtitle(const TitleInfo& info)
    {
        auto* sub = _getSubTitle(_activeAmount++);

        sub->setTitle(info);
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
        sub->setSize({200u, 25u});
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
