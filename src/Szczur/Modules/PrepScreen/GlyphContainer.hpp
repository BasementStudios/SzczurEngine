#pragma once

#include <unordered_map>
#include <map>
#include <string>

#include "GlyphTypes.hpp"

namespace rat
{
    class GlyphContainer
    {
    public:
        GlyphContainer();

        void addGlyph(GlyphID type, size_t addon = 1);
        void removeGlyph(GlyphID type, size_t sub = 1);
        size_t getGlyphAmount(GlyphID type);

        bool hasRequirements(const std::map<GlyphID, size_t>& requirements) const;
    private:
        std::unordered_map<GlyphID, size_t> _glyphs;
    };
}