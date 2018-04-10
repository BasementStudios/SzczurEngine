#pragma once

#include <set>
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
                for(const ColoredPP& requirement : _typesRequirement)
                {
                    if(!source.hasProperColoredPP( requirement )) return false;
                }
                return true;
            }
            void setCost(amount_t newCost)
            {
                _ppCost = newCost;
            }
            void addColorRequirement(const ColoredPP& requirement)
            {
                addColorRequirement(requirement.type, requirement.power);
            }
            void addColorRequirement(const std::string color, power_t power)
            {
                for(size_t i = 1; i <= 2; i++)
                {
                    auto found = _typesRequirement.find({color, i});
                    if(found == _typesRequirement.end()) continue;                    
                    else if(found->power == power) return;
                    else _typesRequirement.erase(found);
                }
                _typesRequirement.emplace(color, power);
            }

            void resetColorRequirements()
            {
                _typesRequirement.clear();
            }

            void buyFrom(PPContainer& source)
            {
                assert(canBeBoughtFrom(source));
                source.removePP(_ppCost);
                std::cout << "New Amount Of PP: " << source.getPPAmount() << "\n";
                _hasBeenBought = true;
            }

            void returnTo(PPContainer& source)
            {
                source.addPP(_ppCost);
                std::cout << "New Amount Of PP: " << source.getPPAmount() << "\n";
                _hasBeenBought = false;
            }

            bool isBought() const {
                return _hasBeenBought;
            }    

            amount_t getNumberOfRequirements() const{
                return _typesRequirement.size();
            } 

        private:
            std::set<ColoredPP> _typesRequirement;
            amount_t _ppCost;
            bool _hasBeenBought{false};
        
    };
}