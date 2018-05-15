#include "SkillCost.hpp"

#include "Szczur/Modules/PrepScreen/ResourcesContainer.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    void SkillCost::addRequirement(GlyphID glyph, amount_t power)
    {
        auto found = _glyphsRequired.find(glyph);
        if(found == _glyphsRequired.end())
        {
            _glyphsRequired.emplace(glyph, power);
        }
        else
        {
            found->second = power;
        }
    }
    std::set<GlyphID> SkillCost::getGlyphs() const
    {
        std::set<GlyphID> glyphs;
        glyphs.clear();
        for(const auto& [glyph, power] : _glyphsRequired)
        {
            glyphs.emplace(glyph);
        }
        return glyphs;
    }
    void SkillCost::resetRequirements()
    {
        _glyphsRequired.clear();
    }

    void SkillCost::setCost(amount_t amount)
    {
        _cost = amount;
    }
    size_t SkillCost::getCost() const
    {
        return _cost;
    }

    size_t SkillCost::getNumberOfRequirements() const
    {
        return _glyphsRequired.size();
    }

    bool SkillCost::canBeBoughtFrom(const ResourcesContainer& container) const
    {
        const auto& ppCont = container.ppContainer;
        const auto& glyphCont = container.glyphContainer;

        if(_cost > ppCont.getAmount()) return false;
        return glyphCont.hasRequirements(_glyphsRequired);
    }
}