#pragma once

#include <map>
#include <algorithm>
#include <initializer_list>

#include "PPContainer.hpp"
#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    class PPCost
    {
        using amount_t = size_t;
        using power_t = size_t;
       
        public:
            bool canBeBoughtFrom(const PPContainer& source)
            {
                if(source.getPPAmount() < _ppCost) return false;
                for(const auto[color, power] : _typesRequirement)
                {
                    if(!source.hasProperColoredPP( {color, power} )) return false;
                }
                return true;
            }
            bool hasColorRequirement(const std::string& color) const
            {
                auto found = _typesRequirement.find(color);
                return (found != _typesRequirement.end());                  
            }
            void setCost(amount_t newCost)
            {
                _ppCost = newCost;
            }
            amount_t getCost() const 
            {
                return _ppCost;
            }
            void addColorRequirement(const ColoredPP& requirement)
            {
                addColorRequirement(requirement.type, requirement.power);
            }
            void addColorRequirement(const std::string color, power_t power)
            {
                auto found = _typesRequirement.find(color);
                if(found == _typesRequirement.end()) _typesRequirement.emplace(color, power);             
                else found->second = power;
            }

            void resetColorRequirements()
            {
                _typesRequirement.clear();
            }

            void buyFrom(PPContainer& source)
            {
                //assert(canBeBoughtFrom(source));
                source.removePP(_ppCost);
                _hasBeenBought = true;
            }

            void returnTo(PPContainer& source)
            {
                source.addPP(_ppCost);
                _hasBeenBought = false;
            }

            bool isBought() const {
                return _hasBeenBought;
            }    

            amount_t getNumberOfRequirements() const{
                return _typesRequirement.size();
            } 

            auto begin() const {
                return _typesRequirement.begin();
            }
            auto end() const {
                return _typesRequirement.end();
            }

        private:
            std::map<std::string, size_t> _typesRequirement;
            amount_t _ppCost;
            bool _hasBeenBought{false};        
    };
}