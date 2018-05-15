#include "GlyphContainer.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    GlyphContainer::GlyphContainer()
    {
        GlyphTypes types;
        GlyphesConverter converter;
        _glyphs.clear();
        for(auto& type : types)
        {
            _glyphs.emplace(converter.toEnum(type), 0);
        }
    }

    void GlyphContainer::addGlyph(GlyphID type, size_t addon)
    {
        auto found = _glyphs.find(type);
        if(found == _glyphs.end())
        {
            LOG_ERROR(this, "Glyph is not valid." );
            return;
        }
        found->second += addon;
    }
    void GlyphContainer::removeGlyph(GlyphID type, size_t sub)
    {
        auto found = _glyphs.find(type);
        if(found == _glyphs.end())
        {
            LOG_ERROR(this, "Glyph is not valid." );
            return;
        }
        auto& amount = found->second;
        if(amount < sub)
        {
            GlyphesConverter converter;            
            LOG_ERROR(this, "Trying remove glyphs \"" + converter.toString(type) + "\" that not exist" );
            return;
        }
        amount -= sub;
    }
    size_t GlyphContainer::getGlyphAmount(GlyphID type)
    {
        auto found = _glyphs.find(type);
        if(found == _glyphs.end())
        {
            LOG_ERROR(this, "Glyph is not valid." );
            return 0;
        }
        return found->second;
    }
    bool GlyphContainer::hasRequirements(const std::map<GlyphID, size_t>& requirements) const
    {
        for(auto& [glyph, amount] : requirements)
        {
            if(_glyphs.find(glyph)->second < amount) return false;
        }
        return false;
    }
}