#include "GlyphArea.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/ListWidget.hpp"

namespace rat
{
    GlyphArea::GlyphArea(PrepScreen& prepScreen)
    :
    BaseBar([]{ auto* base = new ListWidget; /*base->makeHorizontal();*/ base->setPropBetweenPad(0.03f); return base;}())
    {
        //setPropOrigin(0.5f, 0.f);
        //setPropSize(0.768f, 0.f);
        //_getBase()->setPropPosition(0.f , 0.5f);

        GlyphTypes types;
        GlyphesConverter converter;
        _glyphBars.reserve(4);
        for(auto& type : types)
        {
            auto glyphBar = std::make_unique<GlyphBar>(prepScreen);
            glyphBar->setType(converter.toEnum(type));
            _addBar(glyphBar);
            _glyphBars.emplace(converter.toEnum(type), std::move(glyphBar));
        }
    }

    void GlyphArea::initAssetsViaGUI(GUI& gui)
    {
        for(auto& [key, glyphBar] : _glyphBars)
        {
            glyphBar->initAssetsViaGUI(gui);
        }
    }

    void GlyphArea::setGlyph(GlyphID id, size_t activated, size_t total)
    {
        auto found = _glyphBars.find(id);
        auto& bar = found->second;
        bar->setAmount(activated, total);
    }
}