#include "GlyphTypes.hpp"

namespace rat
{
    GlyphTypes::GlyphTypes()
    :
    _types( {"Wrath", "Wearines", "Fear", "Desperation" } )
    {
    }

    std::string GlyphesConverter::toString(GlyphID glyphType) const
    {
        switch (glyphType)
        {
            case GlyphID::Wrath:
                return "Wrath";
            break;

            case GlyphID::Wearines:
                return "Wearines";
            break;
            
            case GlyphID::Fear:
                return "Fear";
            break;

            case GlyphID::Desperation:
                return "Desperation";
            break;

            default:
                return "";
            break;
        }
    }

    GlyphID GlyphesConverter::toEnum(const std::string& glyphType) const
    {
        if(glyphType == "Wrath") return GlyphID::Wrath;
        else if(glyphType == "Wearines") return GlyphID::Wearines;
        else if(glyphType == "Fear") return GlyphID::Fear;
        else if(glyphType == "Desperation") return GlyphID::Desperation;

        return GlyphID::Wrath;
    }
}