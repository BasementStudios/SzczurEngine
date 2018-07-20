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

        void activateGlyph(GlyphID type, size_t addon = 1);
        void deactivateGlyph(GlyphID type, size_t sub = 1);

        size_t getGlyphTotalAmount(GlyphID type) const;
        size_t getGlyphAmount(GlyphID type) const;

        bool hasRequirements(const std::map<GlyphID, size_t>& requirements) const;
    private:
        struct GlyphAmount { size_t total{0}; size_t amount{0}; };
        std::unordered_map<GlyphID, GlyphAmount> _glyphs;
    };
}