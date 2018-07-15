#include "Skill.hpp"

#include <cassert>

#include "Szczur/Utility/Logger.hpp"

#include "Szczur/Modules/PrepScreen/ResourcesContainer.hpp"

#include "../GlyphTypes.hpp"

namespace rat
{
    using namespace nlohmann;

    Skill::Skill(const std::string& name)
        :
        _name(name)
        {}

    Skill::Skill(json& j)
    {
        setName(j["name"]);
        setPPCost(j["ppCost"]);
        setProfession(j["profession"]);
        setTexturePath(j["iconPath"]);
        _loadGlyphsFromJson(j["glyphes"]);
    }

    void Skill::_loadGlyphsFromJson(nlohmann::json& j)
    {
        GlyphesConverter converter;
        _cost.resetRequirements();
        for(auto it = j.begin(); it != j.end(); ++it)
        {
            auto glyphID = converter.toEnum(it.key());
            size_t power = it.value();

            addRequirement(glyphID, power);
        }
    }

    void Skill::setName(const std::string& name)
    {
        _name = name;
    }
    void Skill::setStringID(const std::string& nameID)
    {
        _id = nameID;
    }
    const std::string& Skill::getStringID() const
    {
        return _id;
    }

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

    bool Skill::hasGlyphs(const std::set<GlyphID>& colors) const
    {
        return _cost.getGlyphs() == colors;
    }
}