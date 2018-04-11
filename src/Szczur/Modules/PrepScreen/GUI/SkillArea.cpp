#include "SkillArea.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/ScrollAreaWidget.hpp"
#include "../PPColors.hpp"
#include "../ProfessionTypes.hpp"

namespace rat
{
    SkillArea::SkillArea(GrayPPBar& sourceBar)
    :
    _sourceBar(sourceBar),
    _curentColor("Fire"),
    _curentProfession("Mele")
    {
        std::cout << "SkillArea initing...\n";
        ProfessionTypes professions;
        PPColors ppColors;

        _base = new ScrollAreaWidget;
        _base->setSize(500.f, 100.f);

        for(auto& profession : professions)
        {
            _skillBars.emplace(profession, SkillColors_t());
            for(auto& ppColor : ppColors)
            {
                _skillBars[profession].emplace(ppColor, SkillBars_t());
            }
             _skillBars[profession].emplace("Mixed", SkillBars_t());
        }
    }

    void SkillArea::initAssetsViaGUI(GUI& gui)
    {
        gui.addAsset<sf::Texture>("assets/PrepScreen/skillBar.png");
        _textureBar = gui.getAsset<sf::Texture>("assets/PrepScreen/skillBar.png");

        gui.addAsset<sf::Texture>("assets/PrepScreen/skillBarLocked.png");
        _textureLocked = gui.getAsset<sf::Texture>("assets/PrepScreen/skillBarLocked.png");

        for(auto& profs : _skillBars)
            for(auto& colors : profs.second)
                for(auto& skillBar : colors.second)
                {
                    skillBar.setBarTexture(_textureBar, _textureLocked);
                    const auto& iconPath = skillBar.getIconPath();
                    gui.addAsset<sf::Texture>(iconPath);
                    skillBar.setIconTexture(gui.getAsset<sf::Texture>(iconPath));
                    
                }

    }
    

    void SkillArea::initViaSkillCodex(SkillCodex& skillCodex)
    {
        for(auto& [name, skill] : skillCodex)
        {
            std::cout << name << "\n";
            _addSkillBar(skill.get());
        }

    }

    void SkillArea::_addSkillBar(Skill* skill)
    {
        const auto& color = skill->getColor();
        const auto& prof = skill->getProfession();

        std::cout << skill->getName() << " : " << prof << " : " << color << "\n";

        SkillBar skillBar(_sourceBar);
        skillBar.setSkill(skill);
        skillBar.setParent(_base);
        skillBar.deactivate();

        auto& suitableContainer = _skillBars[prof][color];

        size_t i = suitableContainer.size();
        skillBar.setPosition(0.f, float(i) * 80.f);
        suitableContainer.emplace_back(std::move(skillBar));
    }

    void SkillArea::setParent(Widget* parent)
    {
        parent->add(_base);
    }

    void SkillArea::activate(const std::string& profession, const std::string& color)
    {
        setProfession(profession);
        setColor(color);
    }
    void SkillArea::hideAll()
    {
        for(auto& profs : _skillBars)
            for(auto& colors : profs.second)
                for(auto& skillBar : colors.second)
                {
                    skillBar.deactivate();
                }
    }
    void SkillArea::setColor(const std::string& color)
    {
        _hide(_curentProfession, _curentColor);
        _curentColor = color;
        _active(_curentProfession, _curentColor);
    }
    void SkillArea::setProfession(const std::string& profession)
    {
         _hide(_curentProfession, _curentColor);
         _curentProfession = profession;
         _active(_curentProfession, _curentColor);
    }

    void SkillArea::_hide(const std::string& profession, const std::string& color)
    {
        for(auto& skillBar : _skillBars[profession][color])
        {
            skillBar.deactivate();
        }
    }
    void SkillArea::_active(const std::string& profession, const std::string& color)
    {
        for(auto& skillBar : _skillBars[profession][color])
        {
            skillBar.activate();
        }
    }
    
    
    
}