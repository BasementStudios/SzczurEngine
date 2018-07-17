#include "SkillArea.hpp"

#include <algorithm>
#include <ctime>

#include "Szczur/Modules/PrepScreen/PrepScreen.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/ScrollAreaWidget.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Szczur/Modules/GUI/ListWidget.hpp"

#include "ChosenSkillArea.hpp"
#include "GrayPPArea.hpp"

#include "../Skill/SkillCodex.hpp"

#include "Szczur/Utility/Logger.hpp"



namespace rat
{
    SkillArea::SkillArea(PrepScreen& prep)
    {
        _border = new WindowWidget;
        _addWidget(_border);
        _border->setMainPatchPropSize({0.3f, 0.3f});
        _border->setPropSize(0.463f, 0.85f);

        _skillsScroller = new ScrollAreaWidget;
        _skillsScroller->setPropSize(0.37f, 0.75f); //0.65f
        _skillsScroller->setPropPosition(0.5f, 0.5f);
        _skillsScroller->makeScrollAutoHiding();
        _border->add(_skillsScroller);

        _skillsList = new ListWidget;
        _skillsList->setBetweenPadding(10.f);
        _skillsScroller->add(_skillsList);

        _addBar(_infoBar);
        _infoBar.deactivate();
        _getBase()->setPropPosition(0.5f, 0.f);

        for(size_t i = 0; i < 20; i++)
        {
            auto skillBar = std::make_unique<SkillBar>(prep);
            skillBar->setParent(_skillsList);
            _skillBars.emplace_back(std::move(skillBar));
        }
    }

    void SkillArea::resetScroller()
    {
        _skillsScroller->resetScrollerPositionInTime(0.3f);
    }

    void SkillArea::initAssetsViaGUI(GUI& gui)
    {
        _font = gui.getAsset<sf::Font>("Assets/fonts/NotoMono.ttf");

        _skillsScroller->setPathTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBar.png"));
        _skillsScroller->setScrollerTexture(gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"));
        _skillsScroller->setBoundsTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBound.png"));

        _infoBar.initAssetsViaGUI(gui);

        _border->setTexture(gui.getAsset<sf::Texture>("Assets/PrepScreen/SkillBack.png"), 30);
        

        for(auto& skillBar : _skillBars)
        {
            skillBar->loadAssetsFromGUI(gui);
        }
    }

    void SkillArea::setSkills(const std::vector<const Skill*>& skills)
    {
        for(auto& bar : _skillBars) bar->removeSkill();

        for(size_t i = 0; i < skills.size(); ++i)
        {
            _skillBars[i]->setSkill(skills[i]);
        }

        _skillsScroller->resetScrollerPosition();
    }

    void SkillArea::recalculateSkillsAvailability()
    {
        for(auto& bar : _skillBars)
        {
            if(!bar->hasSkill()) break;
            bar->recalculateAvailability();
        }
    }
}