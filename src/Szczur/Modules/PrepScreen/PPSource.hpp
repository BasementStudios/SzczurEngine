#pragma once

#include <string>
#include <set>

namespace rat
{
    struct ColoredPP
    {
        using power_t = size_t;

        ColoredPP(const std::string& type, power_t power)
        :
        type(type),
        power(power)
        {
        }
        const std::string type;
        const power_t power;

        bool operator<(const ColoredPP& other) const {
            if(type == other.type) return power < other.power;
            return type < other.type;
        }
    };

    class PPSource
    {
        using amount_t = size_t;
        using power_t = size_t;

    public:
        PPSource(const std::string& type, power_t power, amount_t amount = 1) 
             :
        _coloredPP(type, power),
        _ppAmount(amount)
        {
        }

        void setPPAmount(amount_t amount)
        {
            _ppAmount = amount;
        }
        amount_t getPPAmount() const {
            return _ppAmount;
        }

        const ColoredPP& getColoredPP() const {
            return _coloredPP;
        }
        const std::string& getType() const {
            return _coloredPP.type; 
        }
        power_t getPower() const {
            return _coloredPP.power;
        }

        void makeAdded(){
            _hasBeenAdded = true;
        }
        void makeFree(){
            _hasBeenAdded = false;
        }
        bool hasBeenAdded() const {
            return _hasBeenAdded;
        }
        
    private:
        ColoredPP _coloredPP;
        amount_t _ppAmount;

        bool _hasBeenAdded{false};
        static std::set<std::string> _types;
    };
    
}