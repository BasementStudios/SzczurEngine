#include "ProfessionArea.hpp"

#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/ListWidget.hpp"
#include "../ProfessionTypes.hpp"

namespace rat
{
    ProfessionArea::ProfessionArea(SkillArea& skillArea)
    :
    BaseBar([]{ auto* base = new ListWidget; base->setBetweenPadding(5.f); return base;}())
    {
        _initProfBars(skillArea);
    }
    void ProfessionArea::initAssetsViaGUI(GUI& gui)
    {
        for(auto& profBar : _profBars)
        {
            profBar->initAssetsViaGUI(gui);
        }
    }

    void ProfessionArea::_initProfBars(SkillArea& skillArea)
    {
        ProfessionTypes profTypes;
        for(auto& prof : profTypes)
        {
            auto profBar = std::make_unique<ProfessionBar>(skillArea);
            _addBar(profBar);
            profBar->setProfession(prof);

            _profBars.emplace_back(std::move(profBar));
        }
    }
}