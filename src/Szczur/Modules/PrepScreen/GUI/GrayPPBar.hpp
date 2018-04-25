#pragma once

#include "PPBar.hpp"

#include <vector>

namespace rat
{
    class ImageWidget; class GUI;

    class GrayPPBar : public PPBar
    {
    public:
        GrayPPBar(PPContainer& source);

        void recalculate();
        void initTextureViaGui(GUI& gui);

        PPContainer& getSource();
    private:

        std::vector<ImageWidget*> _pps;
        sf::Texture* _texture{nullptr};


        float _dim{40.f}; 

        void _resetPPWidget(ImageWidget* pp);
        void _addPPWidget(size_t amount = 1);

        size_t _activePPsAmount{0};

        void _recalculateAllPPs();
        void _recalculatePPsFromTo(size_t from, size_t to);
        void _deactivatePPsFromTo(size_t from, size_t to);
    };
}