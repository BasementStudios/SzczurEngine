#include "SkillArea.hpp"

#include <algorithm>

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/ScrollAreaWidget.hpp"

#include "Szczur/Utility/Logger.hpp" 

namespace rat
{
    SkillArea::SkillArea(GrayPPBar& sourceBar)
    :
    _sourceBar(sourceBar),
    _chosenColors({"Fire"}),
    _curentProfession("Mele")
    {
        _base = new ScrollAreaWidget;
        _base->setSize(240.f, 400.f);
    }

    void SkillArea::initAssetsViaGUI(GUI& gui)
    {
        gui.addAsset<sf::Texture>("assets/PrepScreen/skillBar.png");
        _textureBar = gui.getAsset<sf::Texture>("assets/PrepScreen/skillBar.png");

        gui.addAsset<sf::Texture>("assets/PrepScreen/skillBarLocked.png");
        _textureLocked = gui.getAsset<sf::Texture>("assets/PrepScreen/skillBarLocked.png");

        gui.addAsset<sf::Font>("assets/fonts/NotoMono.ttf");
        _font = gui.getAsset<sf::Font>("assets/fonts/NotoMono.ttf");

        for(auto& skillBar : _skillBars)
        {
            skillBar->setBarTexture(_textureBar, _textureLocked);
            skillBar->setFont(_font);
            skillBar->loadAssetsFromGUI(gui);
        }
    }
    

    void SkillArea::initViaSkillCodex(SkillCodex& skillCodex)
    {
        _skills.initViaSkillCodex(skillCodex);
        size_t maxSkillBars = _skills.getMaxAmountOfSkills();

        std::cout << "Nmober of skillBars: " << maxSkillBars << "\n";

        _skillBars.clear();
        for(size_t i = 0; i < maxSkillBars; i++)
        {
            auto skillBar = std::make_unique<SkillBar>(*this);
            skillBar->setParent(_base);
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
        auto skills = _skills.getSkills(profession, colors);
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
        std::cout << "Amount: " << newAmount << "\n";
        for(size_t i = 0; i < newAmount; i++)
        {
            auto& skillBar = _skillBars[i];
            skillBar->activate();
            skillBar->setPosition(0.f, float(i) * 80.f);
            std::cout << "SkillBar number " << i +1 << " inited\n";
        }
        for(size_t i = newAmount; i < _activeBarsAmount; i++)
        {
            auto& skillBar = _skillBars[i];
            skillBar->deactivate();
            skillBar->setPosition(0.f, 0.f);
            std::cout << "SkillBar number " << i +1 << ". deactivated\n";            
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
                std::cout << "Recals deactive, bitch\n";
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
    }
}