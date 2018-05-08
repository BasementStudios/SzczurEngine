#pragma once
#include <array>
#include <string>

namespace rat
{
    class GlyphTypes
    {
    public:
    GlyphTypes();
    auto begin() const
    {
        return _types.begin();
    }
    auto end() const
    {
        return _types.end();
    }
    private:
        const std::array<std::string, 4u> _types;
    };
}