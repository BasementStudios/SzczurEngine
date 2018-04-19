#pragma once

#include <SFML/Graphics/Texture.hpp>

#include "../Skill.hpp"

namespace rat
{
    class Widget; class ImageWidget; class TextWidget;
    class GUI;
    class ChosenSkillArea; class SkillArea; class GrayPPBar;

    class ChosenSkillBar
    {
    public:
        ChosenSkillBar(ChosenSkillArea& parentArea);

        void setSkill(Skill* skill);
        void setPosition(float x, float y);
        void move(float x, float y);
        void setParent(Widget* parent);
        void initAssetsViaGUI(GUI& gui);
        bool hasSkill() const;
        bool isFree() const;
        void setSize(float x, float y);
        void swapSkillsWith(ChosenSkillBar& other);

    private:
        ChosenSkillArea& _parentArea;
        SkillArea& _skillArea;
        GrayPPBar& _sourceBar;

        Skill* _skill{nullptr};

        ImageWidget* _base{nullptr};
        ImageWidget* _icon{nullptr};       

        bool _hasSkill{false};
        sf::Vector2f _size{120.f, 120.f};

        void _setIconTexture(sf::Texture* texture);
        void _onClick();

    };
}