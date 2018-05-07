#pragma once

#include "BaseBar.hpp"

#include <SFML/Graphics/Texture.hpp>

#include "../Skill.hpp"

namespace rat
{
    class ImageWidget; class TextWidget;
    class GUI;
    class ChosenSkillArea; class SkillArea; class GrayPPBar;

    class ChosenSkillBar : public BaseBar
    {
    public:
        ChosenSkillBar(ChosenSkillArea& parentArea);

        void setSkill(Skill* skill);
        void initAssetsViaGUI(GUI& gui);
        bool hasSkill() const;
        bool isFree() const;
        void setSize(float x, float y);
        void setSize(const sf::Vector2f& size);
        void swapSkillsWith(ChosenSkillBar& other);

    private:
        ChosenSkillArea& _parentArea;
        SkillArea& _skillArea;
        GrayPPBar& _sourceBar;

        Skill* _skill{nullptr};
        ImageWidget* _icon{nullptr};       

        bool _hasSkill{false};
        sf::Vector2f _size{120.f, 120.f};

        void _setIconTexture(sf::Texture* texture);
        void _onClick();

    };
}