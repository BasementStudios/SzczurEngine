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
            _glyphs.emplace(converter.toEnum(type), GlyphAmount());
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
        found->second.total += addon;
    }
    void GlyphContainer::removeGlyph(GlyphID type, size_t sub)
    {
        auto found = _glyphs.find(type);
        if(found == _glyphs.end())
        {
            LOG_ERROR(this, "Glyph is not valid." );
            return;
        }
        auto& amount = found->second.total;
        if(amount < sub)
        {
            GlyphesConverter converter;            
            LOG_ERROR(this, "Trying remove glyphs \"" + converter.toString(type) + "\" that not exist" );
            return;
        }
        amount -= sub;
    }
    void GlyphContainer::activeGlyph(GlyphID type, size_t addon)
    {
        auto found = _glyphs.find(type);
        if(found == _glyphs.end())
        {
            LOG_ERROR(this, "Glyph is not valid." );
            return;
        }
        if(found->second.total - found->second.amount < addon)
        {
            LOG_ERROR(this, "Not enough glyphs to activate.");
            return;
        }
        found->second.amount += addon;
    }
    void GlyphContainer::deactiveGlyph(GlyphID type, size_t sub)
    {
        auto found = _glyphs.find(type);
        if(found == _glyphs.end())
        {
            LOG_ERROR(this, "Glyph is not valid." );
            return;
        }
        auto& amount = found->second.amount;
        if(amount < sub)
        {
            GlyphesConverter converter;            
            LOG_ERROR(this, "Trying deactive glyphs \"" + converter.toString(type) + "\" that not exist" );
            return;
        }
        amount -= sub;
    }
    size_t GlyphContainer::getGlyphTotalAmount(GlyphID type)
    {
        auto found = _glyphs.find(type);
        if(found == _glyphs.end())
        {
            LOG_ERROR(this, "Glyph is not valid." );
            return 0;
        }
        return found->second.total;
    }
    size_t GlyphContainer::getGlyphAmount(GlyphID type)
    {
        auto found = _glyphs.find(type);
        if(found == _glyphs.end())
        {
            LOG_ERROR(this, "Glyph is not valid." );
            return 0;
        }
        return found->second.amount;
    }
    bool GlyphContainer::hasRequirements(const std::map<GlyphID, size_t>& requirements) const
    {
        for(auto& [glyph, amount] : requirements)
        {
            std::cout << "Glyph Type: " << GlyphesConverter().toString(glyph) << "\n";
            if(_glyphs.find(glyph)->second.amount < amount) return false;
        }
        return true;
    }
}