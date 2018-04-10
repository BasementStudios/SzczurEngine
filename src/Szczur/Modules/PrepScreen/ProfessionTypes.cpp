#include "ProfessionTypes.hpp"

namespace rat
{
        ProfessionTypes::ProfessionTypes()
        :
        _professionTypes(_professions)
        {
        }

        std::vector<std::string> ProfessionTypes::_professions = 
        {
            "Mele", "Range", "Aura", "InerFire"
        };
    
}