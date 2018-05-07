#include "ChosenSkillArea.hpp"

#include <algorithm>
#include <cassert>

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "SkillArea.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    ChosenSkillArea::ChosenSkillArea(SkillArea& parentArea, size_t size)
    :
    _parentArea(parentArea),
    _size(size),
    _widthPadding(70.f),
    _iconSize{246.f, 400.f}
    {
        _skills = new Widget;
        _addWidget(_skills);
        _skills->setPadding(_widthPadding * _scale, 0.f);

        _border = new WindowWidget;
        _addWidget(_border);
        for(size_t i = 0; i < size; i++)
        {
            auto chSkillBar = std::make_unique<ChosenSkillBar>(*this);
            chSkillBar->setParent(_skills);
            chSkillBar->setPosition(_iconSize.x * _scale * float(i), 0.f);
            chSkillBar->setSize(_iconSize * _scale);
            _skillBars.emplace_back(std::move(chSkillBar));
        }
    }

    void ChosenSkillArea::initAssetsViaGUI(GUI& gui)
    {
        _border->setTexture(gui.getAsset<sf::Texture>("Assets/Test/ChosenSkillWindow.png"), 200);
        _border->setScale(_scale, _scale);
        _border->setPatchAmount((unsigned int)_size, 0);
        for(auto& chSkillBar : _skillBars)
        {
            chSkillBar->initAssetsViaGUI(gui);
        }
    }
    void ChosenSkillArea::addSkill(Skill* skill)
    {
        assert(hasFreeSpace());
        auto freeSlot = std::find_if(_skillBars.begin(), _skillBars.end(), [](const std::unique_ptr<ChosenSkillBar>& skillBar){
            return skillBar->isFree();
        });
        (*freeSlot)->setSkill(skill);
        recalculate();
    }
    bool ChosenSkillArea::hasFreeSpace() const
    {
        return std::any_of(_skillBars.begin(), _skillBars.end(), [](const auto& csb){
            return !csb->hasSkill();
        });
    }
    void ChosenSkillArea::recalculate()
    {
        int move = 1;
        for(auto i = _skillBars.begin(); i < _skillBars.end(); ++i)
        {
            auto& skillBar = (*i);
            if(skillBar->hasSkill()) continue;
            bool itsTimeToStop = false;
            while(true)
            {
                auto j = i + move;
                if(j == _skillBars.end())
                {
                    itsTimeToStop = true;
                    break;
                }
                auto& nextBar = *j;
                if(!nextBar->hasSkill())
                {
                    move++;
                    continue;
                } 
                skillBar->swapSkillsWith(*nextBar);
                break;
            }
            if(itsTimeToStop) break;
        }
    }
    SkillArea& ChosenSkillArea::getSkillArea() const
    {
        return _parentArea;
    }
    
}