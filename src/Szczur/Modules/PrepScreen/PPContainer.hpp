#pragma once

#include <map>
#include <functional>
#include <vector>
#include <algorithm>

#include "PPSource.hpp"

namespace rat
{
    class PPContainer
    {
        using amount_t = size_t;
        using power_t = size_t;

    public:
        void add(PPSource& addon)
        {
            assert(!addon.hasBeenAdded());
            const auto& colPP = addon.getColoredPP();
            auto found = _coloredPPs.find(colPP);
            if(found == _coloredPPs.end()){
                _coloredPPs.emplace(colPP, 1);
            }
            else {
                found->second++;
            }
            _amountOfPP += addon.getPPAmount();
            addon.makeAdded();
        }
        void remove(PPSource& addon)
        {
            assert(addon.hasBeenAdded());
            const auto& colPP = addon.getColoredPP();
            auto found = _coloredPPs.find(colPP);
            assert(found != _coloredPPs.end());
            found->second--;
            if(found->second == 0)
            {
                _coloredPPs.erase(colPP);
            }
            assert(_amountOfPP >= addon.getPPAmount());
            _amountOfPP -= addon.getPPAmount();
            addon.makeFree();
        }

        amount_t getPPAmount() const {
            return _amountOfPP;
        }
        bool hasProperColoredPP(const ColoredPP& colPP)
        {
            for(auto i = colPP.power; i <= 2u; i++)
            {
                auto found = _coloredPPs.find( {colPP.type, i} );
                if(found != _coloredPPs.end()) return true;
            }
            return false;
        }
    private:
        std::map<ColoredPP, amount_t> _coloredPPs;
        amount_t _amountOfPP{ 0u };
    };
}