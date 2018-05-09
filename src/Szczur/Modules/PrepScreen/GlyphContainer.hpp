#pragma once

#include <unordered_map>
#include <string>


namespace rat
{

    class GlyphContainer
    {
    public:
        GlyphContainer();

        void addGlyph(const std::string& type, int addon = 1);
        void removeGlyph(const std::string& type, int sub = 1);
        int getGlyphAmount(const std::string& type);
    private:
        std::unordered_map<std::string, int> _glyphs;
    };
}