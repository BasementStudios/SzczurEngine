#pragma once

#include "Szczur/Modules/GUI/Base/BaseBar.hpp"

#include <vector>
#include <memory>

#include "ProfessionBar.hpp"

namespace rat
{
    class PrepScreen;

    class ProfessionArea : public BaseBar
    {
    public:
        ProfessionArea(PrepScreen& prepScreen);
        void initAssetsViaGUI(GUI& gui);
    private:
        std::vector<std::unique_ptr<ProfessionBar>> _profBars;

        void _initProfBars(PrepScreen& prepScreen);
    };
}