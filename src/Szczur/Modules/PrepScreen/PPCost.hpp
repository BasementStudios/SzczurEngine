#pragma once

#include <set>
#include <algorithm>
#include <initializer_list>

#include "PPContainer.hpp"

namespace rat
{
    class PPCost
    {
        using amount_t = size_t;
       
        public:
            bool isBuyable(const PPContainer& source)
            {
                if(source.getPPAmount() < _ppCost) return false;
                
            }
        

        private:
            std::set<ColoredPP> _typesRequirement;
            amount_t _ppCost;
        
    };
}