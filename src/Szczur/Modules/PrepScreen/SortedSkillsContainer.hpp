#pragma once

#include <unordered_map>
#include <set>
#include <vector>
#include <string>

namespace rat
{
    class Skill; class SkillCodex;
    class SortedSkillsContainer
    {
        using Colors_t = std::set<std::string>;
        using Skills_t = std::vector<Skill*>;
        using SkillColors_t = std::unordered_map<size_t, Skills_t>;
        using SkillProfs_t = std::unordered_map<std::string, SkillColors_t>;
    public:
        void initViaSkillCodex(SkillCodex& skillCodex);
        Skills_t getSkills(const std::string profession, const Colors_t& colors) const;
        size_t getMaxAmountOfSkills() const;

    private:
        struct ColorSetHasher
        {
            using Colors_t = std::set<std::string>;            
            size_t operator()(const Colors_t& colors) const;
        };

        SkillProfs_t _skills;
        ColorSetHasher _hasher;
        size_t _maxSkillsAmountInProfession{0};

        void _resortSkills();
        void _recalcMaxSkillsAmountProfession();
    };


}