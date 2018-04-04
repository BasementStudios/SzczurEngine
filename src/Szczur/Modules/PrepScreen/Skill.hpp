#pragma once

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
            assert(_unlocked);
            _cost.buyFrom(source);
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
        }
        sf::Texture* getIcon() {
            return _icon;
        }
        void setIcon(sf::Texture* texture){
            _icon = texture;
        }

        private:
        std::string _name;
        PPCost _cost;
        bool _unlocked{true};
        sf::Texture* _icon{nullptr};
    };
}