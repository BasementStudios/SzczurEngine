#pragma once

#include "Szczur/Modules/GUI/Widget.hpp"
#include "../Skill.hpp"

#include "CostBar.hpp"

namespace rat
{
    class ImageWidget;
    class TextWidget;
    class Widget;
    class GUI;

    class SkillArea; class ChosenSkillArea; class GrayPPBar;

    class SkillBar
    {
    public:
        SkillBar(SkillArea& parentArea);
        void setSkill(Skill* skill);
        void setParent(Widget* parent);
        void setPosition(float x, float y);
        void setPosition(const sf::Vector2f& pos);
        void move(float x, float y);
        void setBarTexture(sf::Texture* mainTexture, sf::Texture* lockTexture);
        const std::string& getIconPath() const;
        void setIconTexture(sf::Texture* icon); 
        void setFont(sf::Font* font);
        void loadAssetsFromGUI(GUI& gui);

        void activate();
        void deactivate();
        bool isActivate() const;
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

        std::vector<ImageWidget*> _coloredPPs;
        Widget* _infoBar{nullptr};
        ImageWidget* _bar{nullptr};
        ImageWidget* _icon{nullptr};
        TextWidget* _nameText{nullptr};

        CostBar _costBar;

        sf::Texture* _textureBar{nullptr};
        sf::Texture* _textureLocked{nullptr};

        static sf::Vector2f _size; 
    };
}