#pragma once

#include "Szczur/Modules/GUI/Base/BaseBar.hpp"

#include <vector>

#include "GrayPPBar.hpp"

namespace rat
{
    struct ResourcesContainer;

    class GrayPPArea : public BaseBar
    {
        using amount_t = size_t;
    public:
        GrayPPArea(ResourcesContainer& source);
        void initAssetsViaGUI(GUI& gui);
        ResourcesContainer& getSource();

        void recalculate();
    private:
        amount_t _avaiblePPsAmount;
        ResourcesContainer& _source;

        WindowWidget* _border{nullptr};
        std::vector<std::unique_ptr<GrayPPBar>> _pps;

        amount_t _activePPsAmount{0};

        sf::Texture* _ppTex{nullptr};
        sf::Texture* _containerTex{nullptr};

        void _addPPBar();
        void _removePPBar();
        void _resetTakenPPBars();
        void _takeNecessaryPPBars();
    };
}