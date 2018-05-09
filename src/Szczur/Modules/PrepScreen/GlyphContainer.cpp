#include "GlyphContainer.hpp"

#include "GlyphTypes.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    GlyphContainer::GlyphContainer()
    {
        GlyphTypes types;
        _glyphs.clear();
        for(auto& type : types)
        {
            _glyphs.emplace(type, 0);
        }
    }

    void GlyphContainer::addGlyph(const std::string& type, int addon)
    {
        auto found = _glyphs.find(type);
        if(found == _glyphs.end())
        {
            LOG_ERROR(this, "Glyph \"" + type + "\" is not valid." );
            return;
        }
        found->second += addon;
    }
    void GlyphContainer::removeGlyph(const std::string& type, int sub)
    {
        auto found = _glyphs.find(type);
        if(found == _glyphs.end())
        {
            LOG_ERROR(this, "Glyph \"" + type + "\" is not valid." );
            return;
        }
        auto& amount = found->second;
        if(amount < sub)
        {
            LOG_ERROR(this, "Trying remove glyphs \"" + type + "\" that not exist" );
            return;
        }
        amount -= sub;
    }
    int GlyphContainer::getGlyphAmount(const std::string& type)
    {
        auto found = _glyphs.find(type);
        if(found == _glyphs.end())
        {
            LOG_ERROR(this, "Glyph \"" + type + "\" is not valid." );
            return 0;
        }
        return found->second;
    }
}