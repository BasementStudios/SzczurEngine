#pragma once

#include <string>
#include <map>

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

        void activate();
        void deactivate();

        sf::Vector2f getPPSize() const;
    private:
        Skill* _skill{nullptr};

        Widget* _base{nullptr};
        std::map<std::string, ImageWidget*> _coloredPPs;
        ImageWidget* _costPP{nullptr};
        TextWidget* _costAmount{nullptr};

        constexpr static float _dim = 30.f;
        constexpr static float _padding = 5.f;

        void _setIconTexture(sf::Texture* texture);
        void _recalculateCostAmount();
        void _recalculateColored();
    };
}