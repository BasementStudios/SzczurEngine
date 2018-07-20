#pragma once

#include <unordered_map>
#include <set>
#include <vector>
#include <string>

#include "../GlyphTypes.hpp"
#include "Skill.hpp"

namespace rat
{
    class Skill; class SkillCodex;
    class SortedSkillsContainer
    {
        using Glyphs_t = std::set<GlyphID>;
        using Skills_t = std::vector<const Skill*>;
        using SkillGlyphs_t = std::unordered_map<size_t, Skills_t>;
        using SkillProfs_t = std::unordered_map<std::string, SkillGlyphs_t>;
    public:
        void initViaSkillCodex(SkillCodex& skillCodex);
        Skills_t getSkills(const std::string& profession, const Glyphs_t& glyphs) const;
        Skills_t getWholeGlyphs(const std::string& profession, const Glyphs_t& glyphs) const;
        Skills_t getWholeProfession(const std::string& profession) const;
        size_t getMaxAmountOfSkills() const;

    private:
        struct GlyphSetHasher
        {
            using Glyphs_t = std::set<GlyphID>;            
            size_t operator()(const Glyphs_t& glyphs) const;
        };

        SkillProfs_t _skills;
        GlyphSetHasher _hasher;
        size_t _maxSkillsAmountInProfession{0};

        void _resortSkills();
        void _recalcMaxSkillsAmountProfession();
        void _sort(Skills_t& skills) const;
        void _sort(Skills_t& skills);
    };


}