#pragma once

#include "Szczur/Modules/GUI/Base/BaseBar.hpp"

#include <memory>

#include "../GlyphContainer.hpp"
#include "GlyphBar.hpp"
#include "../GlyphTypes.hpp"


namespace rat 
{
    class GrayPPArea;

    class GlyphArea : public BaseBar
    {
    public:
        GlyphArea(GrayPPArea& source);

        void initAssetsViaGUI(GUI& gui);

        void setGlyph(GlyphID id, size_t activated, size_t total);
    private:
        GrayPPArea& _sourceArea;

        std::unordered_map<GlyphID, std::unique_ptr<GlyphBar>> _glyphBars;
    };
}