#include "GlyphArea.hpp"

#include "../GlyphTypes.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

namespace rat
{
    GlyphArea::GlyphArea()
    {
        setPropOrigin(0.5f, 0.f);

        _container.addGlyph("Wrath", 2);
        _container.addGlyph("Wearines", 1);
        _container.addGlyph("Fear", 3);
        _container.addGlyph("Desperation", 0);

        GlyphTypes types;
        size_t i = 0;
        for(auto& type : types)
        {
            auto& glyphBar = _glyphBars[i];
            glyphBar = std::make_unique<GlyphBar>(type);
            glyphBar->setPosition(float(i) * 120.f, 0.f);
            glyphBar->setStartAmount(_container.getGlyphAmount(type));
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