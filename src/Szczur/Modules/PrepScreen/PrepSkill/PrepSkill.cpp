#include "PrepSkill.hpp"

#include <cassert>

#include "Szczur/Utility/Logger.hpp"

#include "Szczur/Modules/PrepScreen/ResourcesContainer.hpp"

#include "../GlyphTypes.hpp"

namespace rat
{
    using namespace nlohmann;

    PrepSkill::PrepSkill(const std::string& name)
        :
        _name(name)
        {}

    PrepSkill::PrepSkill(json& j)
    {
        setName(j["name"]);
        setPPCost(j["ppCost"]);
        setProfession(j["profession"]);
        setTexturePath(j["iconPath"]);
        _loadGlyphsFromJson(j["glyphes"]);
    }

    void PrepSkill::_loadGlyphsFromJson(nlohmann::json& j)
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

    void PrepSkill::setName(const std::string& name)
    {
        _name = name;
    }
    void PrepSkill::setStringID(const std::string& nameID)
    {
        _id = nameID;
    }
    const std::string& PrepSkill::getStringID() const
    {
        return _id;
    }

    const std::string& PrepSkill::getName() const 
    {
        return _name;
    }
    const SkillCost& PrepSkill::getCostInfo() const 
    {
        return _cost;
    }
    void PrepSkill::unlock()
    {
        _unlocked = true;
    }
    bool PrepSkill::isUnlocked() const 
    {
        return _unlocked;
    }
    void PrepSkill::lock()
    {
        _unlocked = false;
    }
    bool PrepSkill::isBought() const 
    {
        return _isBought;
    }

    void PrepSkill::setPPCost(amount_t cost)
    {
        _cost.setCost(cost);
    }

    void PrepSkill::addRequirement(GlyphID glyph, power_t power)
    {
        _cost.addRequirement(glyph, power);
    }

    void PrepSkill::setRequirements(std::initializer_list<std::pair<GlyphID, power_t>> requirements)
    {
        _cost.resetRequirements();
        for(const auto& [glyph, power] : requirements)
        {
            addRequirement(glyph, power);
        }
    }

    void PrepSkill::setProfession(const std::string& profession)
    {
        _profession = profession;
    }

    const std::string PrepSkill::getProfession() const
    {
        return _profession;
    }

    const std::set<GlyphID> PrepSkill::getGlyphs() const
    {
        return _cost.getGlyphs();
    }

    void PrepSkill::setTexturePath(const std::string& texturePath)
    {
        _texturePath = texturePath;
    }
    const std::string& PrepSkill::getTexturePath() const{
        return _texturePath;
    }
    void PrepSkill::setTexture(sf::Texture* texture){
        assert(texture);
        _iconTexture = texture;
    }

    sf::Texture* PrepSkill::getTexture() const{
        return _iconTexture;
    }

    bool PrepSkill::hasGlyphs(const std::set<GlyphID>& colors) const
    {
        return _cost.getGlyphs() == colors;
    }
}