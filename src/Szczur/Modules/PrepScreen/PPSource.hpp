#include "ColoredPP.hpp"

namespace rat
{
    class PPSource
    {
    public:
        PPSource(const std::string& type, size_t power, size_t amount = 1) 
             :
        _type(type, power),
        _ppAmount(amount)
        {
        }

        void setPPAmount(size_t amount)
        {
            _ppAmount = amount;
        }

        const ColoredPP& getType() const {
            return _type; 
        }
        size_t getPPAmount() const {
            return _ppAmount;
        }
        const ColoredPP& getColoredPP() const{
            return _type;
        }
        size_t getPower()const {
            return _type.getPower();
        }

    private:
        ColoredPP _type;
        size_t _ppAmount;
    };
    
}