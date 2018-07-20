#include "SkillArea.hpp"

#include <algorithm>
#include <ctime>

#include "Szczur/Modules/PrepScreen/PrepScreen.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/ScrollAreaWidget.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Szczur/Modules/GUI/ListWidget.hpp"

#include "Szczur/Modules/PrepScreen/PrepSkill/SkillCodex.hpp"

#include "Szczur/Utility/Logger.hpp"



namespace rat
{
    SkillArea::SkillArea(PrepScreen& prep)
    {
        _border = new WindowWidget;
        _addWidget(_border);
        _border->setMainPatchPropSize({0.5f, 0.5f});
        _border->setPropPadding(0.01f, 0.01f);
        //_border->setPropSize(0.15f, 0.9f);

        // _skillsScroller = new ScrollAreaWidget;
        // _skillsScroller->setPropSize(0.15f, 0.75f); //0.65f
        // _skillsScroller->setPropPosition(0.5f, 0.5f);
        // _skillsScroller->makeScrollAutoHiding();
        //_border->add(_skillsScroller);

        _skillsList = new ListWidget;
        _skillsList->setPropBetweenPad(0.03f);
        _skillsList->setPropPosition(0.5f, 0.5f);
        _border->add(_skillsList);


        for(size_t i = 0; i < 5; i++)
        {
            auto skillBar = std::make_unique<SkillSlot>(prep);
            skillBar->setParent(_skillsList);
            _skillBars.emplace_back(std::move(skillBar));
        }

        _getBase()->setPropPosition(0.f, 0.5f);
    }

    void SkillArea::resetScroller()
    {
        //_skillsScroller->resetScrollerPositionInTime(0.3f);
    }

    void SkillArea::initAssetsViaGUI(GUI& gui)
    {
        _font = gui.getAsset<sf::Font>("Assets/fonts/NotoSerif-Regular.ttf");

        // _skillsScroller->setPathTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBar.png"));
        // _skillsScroller->setScrollerTexture(gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"));
        // _skillsScroller->setBoundsTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBound.png"));

        _border->setTexture(gui.getAsset<sf::Texture>("Assets/PrepScreen/BackgroundEQ.png"), 30);
        

        for(auto& skillBar : _skillBars)
        {
            skillBar->loadAssetsFromGUI(gui);
        }
    }

    void SkillArea::setSkills(const std::vector<const PrepSkill*>& skills)
    {
        for(auto& bar : _skillBars) bar->removeSkill();

        for(size_t i = 0; i < skills.size(); ++i)
        {
            _skillBars[i]->setSkill(skills[i]);
        }
        recalculateSkillsAvailability();
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