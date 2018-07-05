#pragma once 

#include <array>
#include <string>

namespace rat
{
    class ProfessionTypes
    {
    public:
        ProfessionTypes();

        auto begin() const
        {
            return _professionTypes.begin();
        }
        
        auto end() const
        {
            return _professionTypes.end();
        }
    private:
        const std::array<std::string, 3u> _professionTypes;
    };
}