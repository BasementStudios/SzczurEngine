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
        using amount_t = size_t;
    public:
        GrayPPArea();
        void initAssetsViaGUI(GUI& gui);

        void setPPs(const size_t activated);
        void setMaxPPs(size_t maxPPs);

        void dimPPs(size_t amount);
        void undimPPs();
    private:
        amount_t _slots{0};
        amount_t _activated{0};

        WindowWidget* _border{nullptr};
        ListWidget* _ppsList{nullptr};
        std::vector<std::unique_ptr<GrayPPBar>> _pps;

        sf::Texture* _slotTex{nullptr};

        void _addPPBar();
        void _removePPBar();
        void _resetTakenPPBars();
    };
}