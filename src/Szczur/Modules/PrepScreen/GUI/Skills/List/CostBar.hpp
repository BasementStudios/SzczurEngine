#pragma once

#include "Szczur/Modules/GUI/Base/BaseBar.hpp"

#include <string>
#include <map>

#include "Szczur/Modules/PrepScreen/Skill/Skill.hpp"
#include "Szczur/Modules/PrepScreen/GlyphTypes.hpp"

namespace rat
{
    class GUI; class Widget; class ImageWidget; class TextWidget;

    class CostBar : public BaseBar
    {
    public:
        CostBar();
        void loadAssetsFromGUI(GUI& gui);
        void setSkill(const Skill* skill);
        void removeSkill();
        void setWidth(float width);

        sf::Vector2f getPPSize() const;
    private:
        const Skill* _skill{nullptr};

        std::map<GlyphID, ImageWidget*> _glyphs;
        ImageWidget* _costPP{nullptr};
        TextWidget* _costAmount{nullptr};

        float _dim{30.f};
        float _padding{5.f};
        float _width{100.f};

        size_t _numOfActivated{0};

        void _setIconTexture(sf::Texture* texture);
        void _recalculateCostAmount();
        void _recalculateGlyphs();
        void _recalculate();
    };
}