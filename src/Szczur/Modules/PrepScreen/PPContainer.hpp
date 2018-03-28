#pragma once

#include <set>
#include <functional>
#include <vector>
#include <algorithm>

#include "PPSource.hpp"

namespace rat
{
class PPContainer
{

public:
    void updateBySources(const std::vector<PPSource>& sources)
    {
        _amountOfPP = 0;
        for(const PPSource& source : sources)
        {
            const auto& sourceColor = source.getColoredPP();
            auto found = _types.find(sourceColor);
            if(found == _types.end()){
                _types.insert(sourceColor);
            }
            else if(found->getPower < sourceColor.getPower()){
                _types.erase(found);
                _types.
            }
            _amountOfPP += source.getPPAmount();
        }
    }
private:
    std::set<ColoredPP, std::greater<ColoredPP>> _types;
    size_t _amountOfPP{ 0u };
};
}