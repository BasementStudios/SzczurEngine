#include "ProfessionArea.hpp"

#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/ListWidget.hpp"
#include "../ProfessionTypes.hpp"

namespace rat
{
    ProfessionArea::ProfessionArea()
    :
    BaseBar([]{ auto* base = new ListWidget; base->setAutoBetweenPadding(); base->setPropSize(0.f, 0.7f); return base;}())
    {
        _initProfBars();
    }
    void ProfessionArea::initAssetsViaGUI(GUI& gui)
    {
        for(auto& profBar : _profBars)
        {
            profBar->initAssetsViaGUI(gui);
        }
    }

    void ProfessionArea::_initProfBars()
    {
        ProfessionTypes profTypes;
        for(auto& prof : profTypes)
        {
            auto profBar = std::make_unique<ProfessionBar>();
            _addBar(profBar);
            profBar->setProfession(prof);

            _profBars.emplace_back(std::move(profBar));
        }
    }
}