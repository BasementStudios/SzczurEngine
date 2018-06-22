#pragma once

#include "Szczur/Modules/GUI/Base/BaseBar.hpp"

#include <memory>

#include "../GlyphContainer.hpp"
#include "GlyphBar.hpp"
#include "../GlyphTypes.hpp"


namespace rat 
{
    class PrepScreen;

    class GlyphArea : public BaseBar
    {
    public:
        GlyphArea(PrepScreen& prepScreen);

        void initAssetsViaGUI(GUI& gui);

        void setGlyph(GlyphID id, size_t activated, size_t total);
    private:
        std::unordered_map<GlyphID, std::unique_ptr<GlyphBar>> _glyphBars;
    };
}