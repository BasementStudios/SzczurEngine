#include "SortedSkillsContainer.hpp"

#include <functional>
#include <algorithm>

#include "Skill.hpp"
#include "SkillCodex.hpp"

#include "Szczur/Utility/Logger.hpp" 

namespace rat
{
    size_t SortedSkillsContainer::ColorSetHasher::operator()(const Colors_t& colors) const
    {
        size_t seed = 0;
        if(colors.size() == 0) return seed;
        std::hash<std::string> hasher;
        for(const auto& color : colors)
        {
            seed ^= hasher(color) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
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
                const auto colors = skill->getColors();
                size_t key = _hasher(colors);
                auto& suitableContainer = _skills[prof][key];
                suitableContainer.emplace_back(skill.get());
            }
        }
        _resortSkills();
        _recalcMaxSkillsAmountProfession();
    }

    std::vector<Skill*> SortedSkillsContainer::getSkills(const std::string& profession, const Colors_t& colors) const
    {
        auto foundProf = _skills.find(profession);
        if(foundProf == _skills.end()) return {};
        size_t key = _hasher(colors);
        auto foundColor = foundProf->second.find(key);
        if(foundColor == foundProf->second.end()) return {};
        return foundColor->second;
    }

    void SortedSkillsContainer::_resortSkills()
    {
        for(auto& [prof, colors] : _skills)
            for(auto& [key, skills] : colors)
                {
                    _sort(skills);
                }
    }

    void SortedSkillsContainer::_recalcMaxSkillsAmountProfession()
    {
        size_t newMax = 0;
        for(auto& [prof, colors] : _skills)
        {
            size_t skillsAmount = 0;
            for(auto& [key, skills] : colors)
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
        for(auto& [key, colors] : foundProf->second)
            for(Skill* skill : colors)
            {
                wholeProf.emplace_back(skill);
            }
        _sort(wholeProf);
        return wholeProf;
    }

    std::vector<Skill*> SortedSkillsContainer::getWholeColors(const std::string& profession, const Colors_t& colors) const
    {
        if(colors.size() == 0) return getWholeProfession(profession);
        auto foundProf = _skills.find(profession);
        if(foundProf == _skills.end()) return {};
        Skills_t wholeColors;
        for(auto& [key, skills] : foundProf->second)
        {
            if(skills.size() == 0) continue;
            if(skills.front()->hasColors(colors))
            {
                for(auto* skill : skills)
                {
                    wholeColors.emplace_back(skill);
                }
            }
            //std::cout << "Color\n";
        }
        _sort(wholeColors);
        //std::cout << "Sorted\n";
        return wholeColors;
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