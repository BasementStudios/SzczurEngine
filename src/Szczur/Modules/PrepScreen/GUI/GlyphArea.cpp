#include "GlyphArea.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/ListWidget.hpp"

namespace rat
{
    GlyphArea::GlyphArea(GrayPPArea& source)
    :
    _sourceArea(source),
    BaseBar([]{ auto* base = new ListWidget; base->makeHorizontal(); base->setAutoBetweenPadding(); return base;}())
    {
        setPropOrigin(0.5f, 0.f);
        setPropSize(0.6f, 0.f);

        /*
        _container.addGlyph(GlyphID::Wrath, 2);
        _container.addGlyph(GlyphID::Wearines, 1);
        _container.addGlyph(GlyphID::Fear, 3);
        _container.addGlyph(GlyphID::Desperation, 0);*/

        GlyphTypes types;
        GlyphesConverter converter;
        _glyphBars.reserve(4);
        for(auto& type : types)
        {
            auto glyphBar = std::make_unique<GlyphBar>(source);
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