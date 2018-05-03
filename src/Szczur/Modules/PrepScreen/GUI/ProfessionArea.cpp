#include "ProfessionArea.hpp"

#include "Szczur/Modules/GUI/Widget.hpp"
#include "../ProfessionTypes.hpp"

namespace rat
{
    ProfessionArea::ProfessionArea(SkillArea& skillArea)
    {
        _base = new Widget;
        _initProfBars(skillArea);
    }
    void ProfessionArea::setPosition(float x, float y)
    {
        _base->setPosition(x, y);
    }
    void ProfessionArea::initAssetsViaGUI(GUI& gui)
    {
        for(auto& profBar : _profBars)
        {
            profBar->initAssetsViaGUI(gui);
        }
    }
    void ProfessionArea::setParent(Widget* parent)
    {
        parent->add(_base);
    }

    void ProfessionArea::_initProfBars(SkillArea& skillArea)
    {
        ProfessionTypes profTypes;
        size_t i = 0u;
        for(auto& prof : profTypes)
        {
            auto profBar = std::make_unique<ProfessionBar>(skillArea);
            profBar->setParent(_base);
            profBar->setPosition(0.f, profBar->_dim * float(i++));
            profBar->setProfession(prof);

            _profBars.emplace_back(std::move(profBar));
        }
    }
}