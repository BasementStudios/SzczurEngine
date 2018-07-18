/*
#pragma once

#include "Szczur/Modules/GUI/Base/BaseBar.hpp"

#include <vector>
#include <memory>
#include <set>
#include <string>

#include "ColorFilterBar.hpp"

namespace rat
{
    class SkillArea;
    class GUI;

    class ColorFilterArea : public BaseBar
    {
    public:
        ColorFilterArea(SkillArea& parentArea);
        void initAssetsViaGUI(GUI& gui);
        void setWidth(float width);
        SkillArea& getSkillArea();

        void setGlyph(GlyphID);
        void addGlyph(GlyphID);
        void removeGlyph(GlyphID);
        bool canBeAdded(GlyphID);
        void unchooseAll();
        bool hasOnlyOneFilterChosen() const;

    private:
        SkillArea& _parentArea;
        std::vector<std::unique_ptr<ColorFilterBar>> _filters;


        std::set<std::string> _glyphs;
        float _width{200.f};
        void _recalculateFilters();
    };
}

*/