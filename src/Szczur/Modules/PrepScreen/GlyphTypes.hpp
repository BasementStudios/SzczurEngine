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

    enum GlyphID
    {
        Wrath = 54,
        Wearines = 167,
        Fear = 87,
        Desperation = 1345
    };

    struct GlyphesConverter
    {
        std::string toString(GlyphID glyphType) const;
        GlyphID toEnum(const std::string& glyphType) const;
    };

    struct EnumClassLess
    {
        template<typename E>
        bool operator()(E e1, E e2) const { return static_cast<size_t>(e1) < static_cast<size_t>(e2);}
    };
}