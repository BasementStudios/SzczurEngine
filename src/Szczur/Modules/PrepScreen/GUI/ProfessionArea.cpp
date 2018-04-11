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
            const std::string path = "assets/PrepScreen/" + profBar->getProfession() + "Icon.png";
            gui.addAsset<sf::Texture>(path);
            profBar->setTexture(gui.getAsset<sf::Texture>(path));
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
            profBar->setPosition(profBar->dim * float(i++), 0.f);
            profBar->setProfession(prof);

            _profBars.emplace_back(std::move(profBar));
        }
    }
}