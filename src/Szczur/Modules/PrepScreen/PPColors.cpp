#include "PPColors.hpp"

namespace rat
{
    std::set<std::string> PPColors::_staticColors = 
    {
        "Fire", "Water", "Earth",
         "Spirit", "Essence", "Physical", "Air" 
    };

    PPColors::PPColors()
    :
    _colors(_staticColors)
    {
    }
}