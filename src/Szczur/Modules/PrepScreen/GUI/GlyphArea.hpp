#pragma once

#include "BaseBar.hpp"

#include <memory>

#include "../GlyphContainer.hpp"
#include "GlyphBar.hpp"

namespace rat 
{
    class GrayPPArea;

    class GlyphArea : public BaseBar
    {
    public:
        GlyphArea(GrayPPArea& source);

        void initAssetsViaGUI(GUI& gui);
    private:
        GlyphContainer _container;

        GrayPPArea& _sourceArea;

        std::array<std::unique_ptr<GlyphBar>, 4> _glyphBars;
    };
}