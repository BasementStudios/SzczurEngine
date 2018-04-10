#pragma once 

#include <vector>
#include <string>

namespace rat
{
    class ProfessionTypes
    {
    public:
        ProfessionTypes();

        auto begin()
        {
            return _professionTypes.begin();
        }
        
        auto end()
        {
            return _professionTypes.end();
        }
    private:
        static std::vector<std::string> _professions;
        std::vector<std::string>& _professionTypes;
    };
}