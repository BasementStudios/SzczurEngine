#include "ProfessionArea.hpp"

#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/ListWidget.hpp"
#include "Szczur/Modules/PrepScreen/ProfessionTypes.hpp"

namespace rat
{
    ProfessionArea::ProfessionArea(PrepScreen& prepScreen)
    :
    BaseBar([]{ 
        auto* base = new ListWidget; 
        base->setAutoBetweenPadding(); 
        base->setPropPosition(0.5f, 0.f); 
        base->setPropSize(0.463f, 0.f); 
        base->makeHorizontal(); 
        return base;}())
    {
        _initProfBars(prepScreen);
    }
    void ProfessionArea::initAssetsViaGUI(GUI& gui)
    {
        for(auto& profBar : _profBars)
        {
            profBar->initAssetsViaGUI(gui);
        }
    }

    void ProfessionArea::_initProfBars(PrepScreen& prepScreen)
    {
        ProfessionTypes profTypes;
        for(auto& prof : profTypes)
        {
            auto profBar = std::make_unique<ProfessionBar>(prepScreen);
            _addBar(profBar);
            profBar->setProfession(prof);

            _profBars.emplace_back(std::move(profBar));
        }
    }

    void ProfessionArea::unchooseAll()
    {
        for(auto& bar : _profBars) bar->unchoose();
    }
}