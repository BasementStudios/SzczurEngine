#pragma once

#include "BaseBar.hpp"

#include "../Skill/Skill.hpp"

#include "CostBar.hpp"

namespace rat
{
    class SkillArea; class ChosenSkillArea; class GrayPPArea;

    class SkillBar : public BaseBar
    {
    public:
        SkillBar(SkillArea& parentArea);
        void setSkill(Skill* skill);
        const std::string& getIconPath() const;
        void setIconTexture(sf::Texture* icon); 
        void loadAssetsFromGUI(GUI& gui);

        bool isBought() const{
            return _skill->isBought();
        }

    private:
        Skill* _skill{nullptr};

        SkillArea& _parentArea;
        ChosenSkillArea& _chosenArea;
        GrayPPArea& _sourceArea;

        bool _isActivate{true};

        void _onClick();

        CostBar _costBar;

        WindowWidget* _iconWindow{nullptr};
        ImageWidget* _icon{nullptr};

        WindowWidget* _infoBar{nullptr};
        TextWidget* _name{nullptr};

        static sf::Vector2u _size; 
    };
}