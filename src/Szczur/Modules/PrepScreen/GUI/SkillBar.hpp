#pragma once

#include "Szczur/Modules/GUI/Base/BaseBar.hpp"

#include "../Skill/Skill.hpp"

#include "CostBar.hpp"

namespace rat
{
    class SkillArea; class ChosenSkillArea; class GrayPPArea;
    class ListWidget;

    class SkillBar : public BaseBar
    {
    public:
        SkillBar();
        void setSkill(const Skill* skill);
        const std::string& getIconPath() const;
        void setIconTexture(sf::Texture* icon); 
        void loadAssetsFromGUI(GUI& gui);

        void buySkill();
        void removeSkill();
        bool hasSkill() const { return bool(_skill); }

        bool isBought() const{
            return _skill->isBought();
        }

        void recalculateAvailability();

    private:
        const Skill* _skill{nullptr};

        bool _isActivate{true};

        void _onClick();
        void _onHoverIn();
        void _onHoverOut();

        void _buy();
        bool _canBeBought() const;
        bool _isKnownAsBought{false};

        CostBar _costBar;

        WindowWidget* _iconWindow{nullptr};
        ImageWidget* _icon{nullptr};

        WindowWidget* _infoBar{nullptr};
        TextWidget* _name{nullptr};

        static sf::Vector2u _size; 
    };
}