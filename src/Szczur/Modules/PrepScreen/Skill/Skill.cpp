#include "Skill.hpp"

#include <cassert>

#include "Szczur/Modules/PrepScreen/ResourcesContainer.hpp"

#include "../GlyphTypes.hpp"

namespace rat
{
    Skill::Skill(const std::string& name)
        :
        _name(name)
        {}

        const std::string& Skill::getName() const 
        {
            return _name;
        }
        const SkillCost& Skill::getCostInfo() const 
        {
            return _cost;
        }
        void Skill::unlock()
        {
            _unlocked = true;
        }
        bool Skill::isUnlocked() const 
        {
            return _unlocked;
        }
        void Skill::lock()
        {
            _unlocked = false;
        }
        bool Skill::isBought() const 
        {
            return _isBought;
        }
        void Skill::buyFrom(ResourcesContainer& source)
        {
            auto& ppContainer = source.ppContainer;
            auto cost = _cost.getCost();
            ppContainer.takeFrom(cost);
            _isBought = true;
        }

        void Skill::returnCostsTo(ResourcesContainer& source)
        {
            assert(_isBought);
            auto cost = _cost.getCost();
            source.ppContainer.returnTo(cost);
            _isBought = false;
        }

        bool Skill::canBeBoughtFrom(ResourcesContainer& source)
        {
            return _cost.canBeBoughtFrom(source);
        }

        void Skill::setPPCost(amount_t cost)
        {
            _cost.setCost(cost);
        }

        void Skill::addRequirement(GlyphID glyph, power_t power)
        {
            _cost.addRequirement(glyph, power);
        }

        void Skill::setRequirements(std::initializer_list<std::pair<GlyphID, power_t>> requirements)
        {
            _cost.resetRequirements();
            for(const auto& [glyph, power] : requirements)
            {
                addRequirement(glyph, power);
            }
        }

        void Skill::setProfession(const std::string& profession)
        {
            _profession = profession;
        }

        const std::string Skill::getProfession() const
        {
            return _profession;
        }

        const std::set<GlyphID> Skill::getGlyphs() const
        {
            return _cost.getGlyphs();
        }

        void Skill::setTexturePath(const std::string& texturePath)
        {
            _texturePath = texturePath;
        }
        const std::string& Skill::getTexturePath() const{
            return _texturePath;
        }
        void Skill::setTexture(sf::Texture* texture){
            assert(texture);
            _iconTexture = texture;
        }

        sf::Texture* Skill::getTexture() const{
            return _iconTexture;
        }

        bool Skill::hasGlyphs(const std::set<GlyphID>& colors)
        {
            return _cost.getGlyphs() == colors;
        }
}