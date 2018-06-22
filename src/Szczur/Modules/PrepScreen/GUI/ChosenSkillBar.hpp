#pragma once

#include "Szczur/Modules/GUI/Base/BaseBar.hpp"

#include <SFML/Graphics/Texture.hpp>

#include "../Skill/Skill.hpp"

namespace rat
{
    class ImageWidget; class TextWidget;
    class GUI;
    class ChosenSkillArea; class SkillArea; class GrayPPArea;

    class ChosenSkillBar : public BaseBar
    {
    public:
        ChosenSkillBar();

        void setSkill(const Skill* skill);
        void initAssetsViaGUI(GUI& gui);
        bool hasSkill() const;
        bool isFree() const;
        void setSize(float x, float y);
        void setSize(const sf::Vector2f& size);
        void swapSkillsWith(ChosenSkillBar& other);

    private:
        const Skill* _skill{nullptr};
        ImageWidget* _icon{nullptr};       

        bool _hasSkill{false};
        sf::Vector2f _size{120.f, 120.f};

        void _setIconTexture(sf::Texture* texture);
        void _onClick();

    };
}