#include "SkillArea.hpp"

#include <algorithm>


#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/ScrollAreaWidget.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"

#include "ChosenSkillArea.hpp"

#include "Szczur/Utility/Logger.hpp" 
#include <ctime>

namespace rat
{
    SkillArea::SkillArea(GrayPPBar& sourceBar)
    :
    _sourceBar(sourceBar),
    _chosenColors({}),
    _curentProfession("Mele")
    {
        _base = new WindowWidget;
        _base->setPadding(11.f, 11.f);

        _skillsScroller = new ScrollAreaWidget;
        _skillsScroller->setSize(300.f, 400.f);
        _base->add(_skillsScroller);
    }

    void SkillArea::initAssetsViaGUI(GUI& gui)
    {
        _font = gui.getAsset<sf::Font>("Assets/fonts/NotoMono.ttf");

        _skillsScroller->setPathTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBar.png"));
        _skillsScroller->setScrollerTexture(gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"));
        _skillsScroller->setBoundsTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBound.png"));

        _base->setTexture(gui.getAsset<sf::Texture>("Assets/Test/Window.png"), 200);

        for(auto& skillBar : _skillBars)
        {
            skillBar->loadAssetsFromGUI(gui);
        }
    }
    

    void SkillArea::initViaSkillCodex(SkillCodex& skillCodex)
    {
        _skills.initViaSkillCodex(skillCodex);
        size_t maxSkillBars = _skills.getMaxAmountOfSkills();

        _skillBars.clear();
        for(size_t i = 0; i < maxSkillBars; i++)
        {
            auto skillBar = std::make_unique<SkillBar>(*this);
            skillBar->setParent(_skillsScroller);
            _skillBars.emplace_back(std::move(skillBar));
        }
        deactivate();
    }
    

    void SkillArea::setParent(Widget* parent)
    {
        parent->add(_base);
    }

    void SkillArea::activate(const std::string& profession, const std::set<std::string>& colors)
    {
        _curentProfession = profession;
        _chosenColors = colors;
        auto skills = _skills.getWholeColors(profession, colors);
        size_t newBarsAmount = skills.size();
        size_t i = 0;
        for(auto* skill : skills)
        {
            auto& skillBar = _skillBars[i++];
            skillBar->setSkill(skill);
        }
        _initNewSkillBarsAmount(newBarsAmount);
    }
    void SkillArea::_initNewSkillBarsAmount(size_t newAmount)
    {
        for(size_t i = 0; i < newAmount; i++)
        {
            auto& skillBar = _skillBars[i];
            skillBar->activate();
            skillBar->setPosition(0.f, float(i) * 80.f + 600.f);
        }
        for(size_t i = newAmount; i < _activeBarsAmount; i++)
        {
            auto& skillBar = _skillBars[i];
            skillBar->deactivate();
            skillBar->setPosition(0.f, 0.f);
        }
        _activeBarsAmount = newAmount;
        recalculate();
    }
    
    void SkillArea::activate()
    {
        for(size_t i = 0; i < _activeBarsAmount; i++)
        {
            auto& skillBar = _skillBars[i];
            skillBar->activate();
        }
        recalculate();
    }
    void SkillArea::deactivate()
    {
        for(auto& skillBar : _skillBars)
        {
            skillBar->deactivate();
        }
    }
    
    void SkillArea::setColors(const std::set<std::string>& colors)
    {
        activate(_curentProfession, colors);
    }
    void SkillArea::addColor(const std::string& color)
    {
        if(_chosenColors.find(color) == _chosenColors.end())
        {
            _chosenColors.emplace(color);
            setColors(_chosenColors);
        }
    }
    void SkillArea::setProfession(const std::string& profession)
    {
        activate(profession, _chosenColors);

    }

    GrayPPBar& SkillArea::getSourceBar()
    {
        return _sourceBar;
    }

    void SkillArea::initChosenSkillArea(ChosenSkillArea& chosenSkillArea)
    {
        _chosenSkillArea = &chosenSkillArea;
    }
    ChosenSkillArea& SkillArea::getChosenSkillArea() const
    {
        return *_chosenSkillArea;
    }

    
    
    
    void SkillArea::recalculate()
    {
        int moveDir = 0;
        size_t activeIndex = 0;
        for(size_t i = 0; i < _activeBarsAmount; i++)
        {
            auto& skillBar = _skillBars[i];
            bool isBought = skillBar->isBought();
            bool isActivate = skillBar->isActivate();
            if(isActivate && isBought)
            {
                moveDir--;
                skillBar->deactivate();
            }
            else
            {
                if(!isActivate && !isBought)
                {
                    moveDir++;
                    skillBar->activate();
                    skillBar->setPosition(0.f, float(activeIndex) * 80.f);
                }
                else
                {
                    if(moveDir != 0) skillBar->move(0.f, float(moveDir) * 80.f);
                }
                activeIndex++;
            }
        }
        _skillsScroller->calculateSize();
    }
}