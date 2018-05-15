#pragma once

#include <map>
#include <string>
#include <set> 

#include "Szczur/Modules/PrepScreen/GlyphTypes.hpp"

namespace rat
{
    class ResourcesContainer;

    class SkillCost
    {
        using amount_t = size_t;
    public:
        void addRequirement(GlyphID glyph, amount_t power = 1);
        std::set<GlyphID> getGlyphs() const;
        void resetRequirements();

        void setCost(amount_t amount);
        amount_t getCost() const;
        amount_t getNumberOfRequirements() const;

        bool canBeBoughtFrom(const ResourcesContainer& container) const;

        auto begin() const{
            return _glyphsRequired.begin();
        }
        auto end() const{
            return _glyphsRequired.end();
        }

    private:
        amount_t _cost;
        std::map<GlyphID, amount_t> _glyphsRequired;
    };
}