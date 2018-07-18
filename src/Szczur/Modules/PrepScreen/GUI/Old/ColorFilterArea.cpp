/*

#include "ColorFilterArea.hpp"

#include <algorithm>
#include <cassert>

#include "Szczur/Modules/GUI/GUI.hpp"
#include "../GlyphTypes.hpp"

#include "SkillArea.hpp"

namespace rat 
{
    ColorFilterArea::ColorFilterArea(SkillArea& parentArea)
    :
    BaseBar(),
    _parentArea(parentArea)
    {
        const GlyphTypes ppGlyphs;
        for(const auto& ppGlyph : ppGlyphs)
        {
            auto filter = std::make_unique<ColorFilterBar>(*this);
            filter->deactivate();
            filter->setGlyph(ppGlyph);
            _addBar(filter);
            filter->activate();
            _filters.emplace_back(std::move(filter));
        }
        _recalculateFilters();
    }
    

    void ColorFilterArea::initAssetsViaGUI(GUI& gui)
    {
        for(auto& filter : _filters)
        {
            filter->initAssetsViaGUI(gui);
        }
    }
    void ColorFilterArea::setWidth(float width)
    {
        _width = width;
        _recalculateFilters();
    }
    SkillArea& ColorFilterArea::getSkillArea()
    {
        return _parentArea;
    }

    void ColorFilterArea::setGlyph(const std::string& glyph)
    {
        _glyphs.clear();
        _glyphs.emplace(glyph);
        _parentArea.setGlyphs(_glyphs);
    }
    void ColorFilterArea::addGlyph(const std::string& glyph)
    {
        assert(canBeAdded(glyph));
        _glyphs.emplace(glyph);
        _parentArea.setGlyphs(_glyphs);
    }
    void ColorFilterArea::removeGlyph(const std::string& glyph)
    {
        assert(_glyphs.find(glyph) != _glyphs.end());
        _glyphs.erase(glyph);
        _parentArea.setGlyphs(_glyphs);
    }
    bool ColorFilterArea::canBeAdded(const std::string& glyph)
    {
        return _glyphs.size() < 3u;
    }
    void ColorFilterArea::_recalculateFilters()
    {
        if(_filters.size() == 0) return;
        float barsTotalWidth = 0.f;
        for(auto& filter : _filters)
        {
            barsTotalWidth += filter->getDimension();
        }
        float padding = (_width - barsTotalWidth) / float(std::max(_filters.size() - 1u, size_t(1u)));
        float nextX = 0.f;
        for(auto& filter : _filters)
        {
            filter->setPosition(nextX, 0.f);
            nextX += filter->getDimension() + padding;
        }
    }
    void ColorFilterArea::unchooseAll()
    {
        for(auto& filter : _filters)
        {
            filter->unchoose();
        }
    }
    bool ColorFilterArea::hasOnlyOneFilterChosen() const
    {
        return _glyphs.size() == 1u;
    }
    
    
}

*/