#pragma once

#include <initializer_list>

#include "PPcost.hpp"
#include <SFML/Graphics.hpp>

namespace rat
{
    class Skill
    {
        using amount_t = size_t;
        using power_t = size_t;

        public:
        Skill(const std::string& name)
        :
        _name(name)
        {}

        const std::string& getName() const {
            return _name;
        }
        const PPCost& getCostInfo() const {
            return _cost;
        }
        void unlock(){
            _unlocked = true;
        }
        bool isUnlocked() const {
            return _unlocked;
        }
        void lock(){
            _unlocked = false;
        }
        bool isBought() const {
            return _cost.isBought();
        }
        void buyFrom(PPContainer& source)
        {
            //assert(_unlocked);
            _cost.buyFrom(source);
        }

        void returnCostsTo(PPContainer& source)
        {
            assert(_cost.isBought());
            _cost.returnTo(source);
        }

        bool canBeBoughtFrom(PPContainer& source)
        {
            _cost.canBeBoughtFrom(source);
        }

        void setPPCost(amount_t cost)
        {
            _cost.setCost(cost);
        }
        void addRequirement(const std::string color, power_t power = 1)
        {
            _cost.addColorRequirement({color, power});
            if(_cost.getNumberOfRequirements() == 1) 
            {
                _color = color;
            }
            else
            {
                _color = "Mixed";
            }
        }

        void setRequirements(std::initializer_list<std::pair<std::string, power_t>> requirements)
        {
            _cost.resetColorRequirements();
            for(const auto& [color, power] : requirements)
            {
                addRequirement(color, power);
            }
        }

        void setProfession(const std::string& profession)
        {
            _profession = profession;
        }
        const std::string getProfession() const{
            return _profession;
        }

        const std::string& getColor() const{
            return _color;
        }

        void setTexturePath(const std::string& texturePath)
        {
            _texturePath = texturePath;
        }
        const std::string& getTexturePath() const{
            return _texturePath;
        }


        private:
        std::string _name;
        std::string _profession{"Mele"};
        std::string _color{"Mixed"};
        PPCost _cost;
        bool _unlocked{true};
        std::string _texturePath{""};
    };
}