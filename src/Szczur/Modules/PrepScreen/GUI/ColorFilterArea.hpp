#pragma once

#include "BaseBar.hpp"

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

        void setColor(const std::string& color);
        void addColor(const std::string& color);
        void removeColor(const std::string& color);
        bool canBeAdded(const std::string& color);
        void unchooseAll();
        bool hasOnlyOneFilterChosen() const;

    private:
        SkillArea& _parentArea;
        std::vector<std::unique_ptr<ColorFilterBar>> _filters;


        std::set<std::string> _colors;
        float _width{200.f};
        void _recalculateFilters();
    };
}