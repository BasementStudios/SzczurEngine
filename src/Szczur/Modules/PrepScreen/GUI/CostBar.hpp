#pragma once

#include "BaseBar.hpp"

#include <string>
#include <map>

#include "../Skill.hpp"

namespace rat
{
    class GUI; class Widget; class ImageWidget; class TextWidget;

    class CostBar : public BaseBar
    {
    public:
        CostBar();
        void loadAssetsFromGUI(GUI& gui);
        void setSkill(Skill* skill);
        void setGrayPPPosition(float x, float y);
        void setWidth(float width);

        sf::Vector2f getPPSize() const;
    private:
        Skill* _skill{nullptr};

        std::map<std::string, ImageWidget*> _coloredPPs;
        ImageWidget* _costPP{nullptr};
        TextWidget* _costAmount{nullptr};

        float _dim{30.f};
        float _padding{5.f};
        float _width{100.f};

        size_t _numOfActivated{0};

        void _setIconTexture(sf::Texture* texture);
        void _recalculateCostAmount();
        void _recalculateColored();
        void _recalculate();
    };
}