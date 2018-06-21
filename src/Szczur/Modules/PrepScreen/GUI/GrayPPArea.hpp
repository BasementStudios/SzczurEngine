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
        GrayPPArea(ResourcesContainer& source);
        void initAssetsViaGUI(GUI& gui);
        ResourcesContainer& getSource();

        void setPPs(size_t activated, size_t total);
    private:
        amount_t _totalAmount{0};

        ResourcesContainer& _source;

        WindowWidget* _border{nullptr};
        ListWidget* _ppsList{nullptr};
        std::vector<std::unique_ptr<GrayPPBar>> _pps;


        sf::Texture* _ppTex{nullptr};
        sf::Texture* _containerTex{nullptr};

        void _addPPBar();
        void _removePPBar();
        void _resetTakenPPBars();
    };
}