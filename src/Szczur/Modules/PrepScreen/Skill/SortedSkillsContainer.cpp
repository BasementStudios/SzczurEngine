#include "SortedSkillsContainer.hpp"

#include <functional>
#include <algorithm>

#include "Skill.hpp"
#include "SkillCodex.hpp"

#include "Szczur/Utility/Logger.hpp" 

namespace rat
{
    size_t SortedSkillsContainer::GlyphSetHasher::operator()(const Glyphs_t& glyphs) const
    {
        size_t seed = 0;
        if(glyphs.size() == 0) return seed;
        std::hash<int> hasher;

        
        GlyphesConverter converter;
        for(const GlyphID& glyph : glyphs)
        {
            seed ^= hasher(int(glyph)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }

    void SortedSkillsContainer::initViaSkillCodex(SkillCodex& skillCodex)
    {
        _skills.clear();
        for(auto& [name, skill] : skillCodex)
        {
            if(skill->isUnlocked())
            {
                const auto& prof = skill->getProfession();
                const auto glyphs = skill->getGlyphs();
                size_t key = _hasher(glyphs);
                auto& suitableContainer = _skills[prof][key];
                suitableContainer.emplace_back(skill.get());
            }
        }
        _resortSkills();
        _recalcMaxSkillsAmountProfession();
    }

    std::vector<Skill*> SortedSkillsContainer::getSkills(const std::string& profession, const Glyphs_t& glyphs) const
    {
        auto foundProf = _skills.find(profession);
        if(foundProf == _skills.end()) return {};
        size_t key = _hasher(glyphs);
        auto foundGlyph = foundProf->second.find(key);
        if(foundGlyph == foundProf->second.end()) return {};
        return foundGlyph->second;
    }

    void SortedSkillsContainer::_resortSkills()
    {
        for(auto& [prof, glyphs] : _skills)
            for(auto& [key, skills] : glyphs)
                {
                    _sort(skills);
                }
    }

    void SortedSkillsContainer::_recalcMaxSkillsAmountProfession()
    {
        size_t newMax = 0;
        for(auto& [prof, glyphs] : _skills)
        {
            size_t skillsAmount = 0;
            for(auto& [key, skills] : glyphs)
            {
                skillsAmount += skills.size();
            }
            newMax = std::max(newMax, skillsAmount);
        }
        _maxSkillsAmountInProfession = newMax;
    }

    size_t SortedSkillsContainer::getMaxAmountOfSkills() const
    {
        return _maxSkillsAmountInProfession;
    }

    std::vector<Skill*> SortedSkillsContainer::getWholeProfession(const std::string& profession) const
    {
        auto foundProf = _skills.find(profession);
        if(foundProf == _skills.end()) return {};
        Skills_t wholeProf;
        for(auto& [key, glyphs] : foundProf->second)
            for(Skill* skill : glyphs)
            {
                wholeProf.emplace_back(skill);
            }
        _sort(wholeProf);
        return wholeProf;
    }

    std::vector<Skill*> SortedSkillsContainer::getWholeGlyphs(const std::string& profession, const Glyphs_t& glyphs) const
    {
        if(glyphs.size() == 0) return getWholeProfession(profession);
        auto foundProf = _skills.find(profession);
        if(foundProf == _skills.end()) return {};
        Skills_t wholeGlyphs;
        for(auto& [key, skills] : foundProf->second)
        {
            if(skills.size() == 0) continue;
            if(skills.front()->hasGlyphs(glyphs))
            {
                for(auto* skill : skills)
                {
                    wholeGlyphs.emplace_back(skill);
                }
            }
        }
        _sort(wholeGlyphs);
        return wholeGlyphs;
    }

    void SortedSkillsContainer::_sort(Skills_t& skills) const
    {
        std::sort(skills.begin(), skills.end(), [](Skill* lhs, Skill* rhs){
                    return lhs->getCostInfo().getCost() < rhs->getCostInfo().getCost();
                });
    }
    void SortedSkillsContainer::_sort(Skills_t& skills)
    {
        std::sort(skills.begin(), skills.end(), [](Skill* lhs, Skill* rhs){
                    return lhs->getCostInfo().getCost() < rhs->getCostInfo().getCost();
                });
    }
    
    
    
}