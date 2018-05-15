#include "GlyphArea.hpp"

#include "../GlyphTypes.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

namespace rat
{
    GlyphArea::GlyphArea(GrayPPArea& source)
    :
    _sourceArea(source)
    {
        setPropOrigin(0.5f, 0.f);

        _container.addGlyph(GlyphID::Wrath, 2);
        _container.addGlyph(GlyphID::Wearines, 1);
        _container.addGlyph(GlyphID::Fear, 3);
        _container.addGlyph(GlyphID::Desperation, 0);

        GlyphTypes types;
        size_t i = 0;
        GlyphesConverter converter;
        for(auto& type : types)
        {
            auto& glyphBar = _glyphBars[i];
            glyphBar = std::make_unique<GlyphBar>(source);
            glyphBar->setType(converter.toEnum(type));
            glyphBar->setPosition(float(i) * 120.f, 0.f);
            glyphBar->setStartAmount(_container.getGlyphAmount(converter.toEnum(type)));
            _addBar(glyphBar);
            i++;
        }
    }

    void GlyphArea::initAssetsViaGUI(GUI& gui)
    {
        for(auto& glyphBar : _glyphBars)
        {
            glyphBar->initAssetsViaGUI(gui);
        }
    }
}