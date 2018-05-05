#pragma once

#include "BaseBar.hpp"

#include "Szczur/Modules/GUI/Widget.hpp"
#include "../Skill.hpp"

#include "CostBar.hpp"

namespace rat
{
    class ImageWidget; class TextWidget; class Widget; class WindowWidget;
    class GUI;

    class SkillArea; class ChosenSkillArea; class GrayPPBar;

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
        GrayPPBar& _sourceBar;

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