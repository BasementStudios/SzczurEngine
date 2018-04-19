#include "ColorFilterArea.hpp"

#include <algorithm>
#include <cassert>

#include "Szczur/Modules/GUI/GUI.hpp"
#include "../PPColors.hpp"

#include "SkillArea.hpp"

namespace rat 
{
    ColorFilterArea::ColorFilterArea(SkillArea& parentArea)
    :
    BaseBar(),
    _parentArea(parentArea)
    {
        const PPColors ppColors;
        for(const auto& ppColor : ppColors)
        {
            auto filter = std::make_unique<ColorFilterBar>(*this);
            filter->deactivate();
            filter->setColor(ppColor);
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

    void ColorFilterArea::setColor(const std::string& color)
    {
        _colors.clear();
        _colors.emplace(color);
        _parentArea.setColors(_colors);
    }
    void ColorFilterArea::addColor(const std::string& color)
    {
        assert(canBeAdded(color));
        _colors.emplace(color);
        _parentArea.setColors(_colors);
    }
    void ColorFilterArea::removeColor(const std::string& color)
    {
        assert(_colors.find(color) != _colors.end());
        _colors.erase(color);
        _parentArea.setColors(_colors);
    }
    bool ColorFilterArea::canBeAdded(const std::string& color)
    {
        return _colors.size() < 3u;
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
        return _colors.size() == 1u;
    }
    
    
}