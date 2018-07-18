#pragma once

#include "Szczur/Modules/GUI/Base/BaseBar.hpp"

#include <vector>

#include "GrayPPBar.hpp"

namespace rat
{
    struct ResourcesContainer;
    class ListWidget;

    class GrayPPArea : public BaseBar
    {
    public:
        GrayPPArea();
        void initAssetsViaGUI(GUI& gui);

        void setMaximum(int amount);
        void setUnlocked(int amount);
        void setFilled(int amount);

        void dimFilledPPs(int amount);
        void dimEmptyPPs(int amount);
        void undimPPs();
    private:
        int _maximum{0}; 
        int _slots{0};
        int _filled{0};

        WindowWidget* _border{nullptr};
        ListWidget* _ppsList{nullptr};
        std::vector<std::unique_ptr<GrayPPBar>> _pps;

        sf::Texture* _slotTex{nullptr};

        void _addPPBar();
        void _removePPBar();
        void _resetTakenPPBars();
    };
}