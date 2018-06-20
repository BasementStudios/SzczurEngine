#pragma once

#include "Szczur/Modules/GUI/Base/BaseBar.hpp"

#include <vector>
#include <memory>

#include "ProfessionBar.hpp"

namespace rat
{
    class GUI; class Widget;

    class ProfessionArea : public BaseBar
    {
    public:
        ProfessionArea(SkillArea& skillArea);
        void initAssetsViaGUI(GUI& gui);
    private:
        std::vector<std::unique_ptr<ProfessionBar>> _profBars;

        void _initProfBars(SkillArea& skillArea);
    };
}