#pragma once

#include "BaseBar.hpp"

#include <memory>

#include "../GlyphContainer.hpp"
#include "GlyphBar.hpp"

namespace rat 
{
    class GlyphArea : public BaseBar
    {
    public:
        GlyphArea();

        void initAssetsViaGUI(GUI& gui);
    private:
        GlyphContainer _container;

        std::array<std::unique_ptr<GlyphBar>, 4> _glyphBars;
    };
}