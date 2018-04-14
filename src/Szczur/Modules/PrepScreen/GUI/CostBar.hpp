#pragma once

#include <vector>

#include "../Skill.hpp"

namespace rat
{
    class GUI; class Widget; class ImageWidget; class TextWidget;

    class CostBar
    {
    public:
        CostBar();
        void loadAssetsFromGUI(GUI& gui);
        void setSkill(Skill* skill);
        void setParent(Widget* parent);
        void setPosition(float x, float y);
        void setGrayPPPosition(float x, float y);

        sf::Vector2f getPPSize() const;
    private:
        Skill* _skill{nullptr};

        Widget* _base{nullptr};
        std::vector<ImageWidget*> _coloredPPs{nullptr};
        ImageWidget* _costPP{nullptr};
        TextWidget* _costAmount{nullptr};

        constexpr static float _dim = 30.f;
        constexpr static float _padding = 5.f;

        void _setIconTexture(sf::Texture* texture);
        void _recalculateCostAmount();
    };
}