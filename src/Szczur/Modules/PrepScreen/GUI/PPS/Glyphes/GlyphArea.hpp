#pragma once

#include "Szczur/Modules/GUI/Base/BaseBar.hpp"

#include <memory>

#include "Szczur/Modules/PrepScreen/GlyphContainer.hpp"
#include "Szczur/Modules/PrepScreen/GlyphTypes.hpp"
#include "GlyphBar.hpp"


namespace rat 
{
    class PrepScreen;

    class GlyphArea : public BaseBar
    {
    public:
        GlyphArea(PrepScreen& prepScreen);

        void initAssetsViaGUI(GUI& gui);

        void setGlyph(GlyphID id, size_t activated, size_t total);

        void moveToLeft();
        void setPosToCenter();
    private:
        std::unordered_map<GlyphID, std::unique_ptr<GlyphBar>> _glyphBars;
    };
}