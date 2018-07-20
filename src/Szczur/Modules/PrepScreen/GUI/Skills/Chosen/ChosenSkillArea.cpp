#include "ChosenSkillArea.hpp"

#include <algorithm>
#include <cassert>

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/ListWidget.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    ChosenSkillArea:: ChosenSkillArea(PrepScreen& prepScreen, size_t size)
    :
    _size(size)
    {
        _skillsList = new ListWidget;
        _addWidget(_skillsList);

        _skillsList->makeHorizontal();
        _skillsList->setPropBetweenPad(0.015f);

        for(size_t i = 0; i < size; i++)
        {
            auto chSkillBar = std::make_unique<ChosenSkillBar>(prepScreen);
            chSkillBar->setParent(_skillsList);
            _skillBars.emplace_back(std::move(chSkillBar));
        }

        _getBase()->setPropPosition(0.5f, 1.f);
        _getBase()->setPropOrigin(0.5f, 0.5f);
        _getBase()->makeStaticPropPositing();
    }

    void ChosenSkillArea::initAssetsViaGUI(GUI& gui)
    {
        //_border->setTexture(gui.getTexture("Assets/PrepScreen/ChosenSkillArea.png"));
        for(auto& chSkillBar : _skillBars)
        {
            chSkillBar->initAssetsViaGUI(gui);
        }
    }
    void ChosenSkillArea::addSkill(const PrepSkill* skill)
    {
        assert(hasFreeSpace());
        auto freeSlot = std::find_if(_skillBars.begin(), _skillBars.end(), [](const std::unique_ptr<ChosenSkillBar>& skillBar){
            return skillBar->isFree();
        });
        (*freeSlot)->setSkill(skill);
        _ocuppied++;
        //recalculate();
    }
    bool ChosenSkillArea::hasFreeSpace() const
    {
        return _ocuppied < _size;
    }

    void ChosenSkillArea::recalculate()
    {
        for(size_t i = 0, j = 0; j < _skillBars.size(); ++i, ++j)
        {
            auto& skillBar = _skillBars[i];

            if(skillBar->isFree()) break;
            if(skillBar->isSkillUnbought())
            {
                skillBar->removeSkill();
                --_ocuppied;
                ++j;
                bool foundNext{false};
                for(; j < _skillBars.size(); ++j)
                {
                    auto& nextBar = _skillBars[j];
                    if(nextBar->isFree())
                    {
                        foundNext = false;
                        break;
                    }

                    if(nextBar->isSkillUnbought())
                    {
                        nextBar->removeSkill();
                        --_ocuppied;
                        continue;
                    }

                    foundNext = true;
                    break;
                }
                if(!foundNext) break;
                skillBar->swapSkillsWith(*_skillBars[j]);
            }
        }
    }
}