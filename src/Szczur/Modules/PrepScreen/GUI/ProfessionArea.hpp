#pragma once

#include <vector>
#include <memory>

#include "ProfessionBar.hpp"

namespace rat
{
    class GUI; class Widget;

    class ProfessionArea
    {
    public:
        ProfessionArea(SkillArea& skillArea);
        void setPosition(float x, float y);
        void initAssetsViaGUI(GUI& gui);
        void setParent(Widget* parent);
    private:
        Widget* _base{nullptr};
        std::vector<std::unique_ptr<ProfessionBar>> _profBars;

        void _initProfBars(SkillArea& skillArea);

    };
}