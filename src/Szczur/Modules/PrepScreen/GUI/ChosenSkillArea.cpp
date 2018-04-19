#include "ChosenSkillArea.hpp"

#include <algorithm>
#include <cassert>

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "SkillArea.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    ChosenSkillArea::ChosenSkillArea(SkillArea& parentArea, size_t size)
    :
    _parentArea(parentArea),
    _size(size)
    {
        _base = new Widget;
        for(size_t i = 0; i < size; i++)
        {
            auto chSkillBar = std::make_unique<ChosenSkillBar>(*this);
            chSkillBar->setParent(_base);
            chSkillBar->setPosition(float(i)*120.f, 0.f);
            _skillBars.emplace_back(std::move(chSkillBar));
        }
    }

    void ChosenSkillArea::setPosition(float x, float y)
    {
        _base->setPosition(x, y);
    }
    void ChosenSkillArea::setParent(Widget* parent)
    {
        parent->add(_base);
    }
    void ChosenSkillArea::initTexturesViaGUI(GUI& gui)
    {
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
        std::cout << "freeSlot found\n";
        (*freeSlot)->setSkill(skill);
        std::cout << "freeSlot skill added\n";
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