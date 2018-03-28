#pragma once

#include <string>
#include <cassert>

namespace rat
{
    class ColoredPP
    {
    public:
        ColoredPP(const std::string& type, size_t power) :
        _type(type)
        {
            assert(power==1||power==2);
            this->_power = power;
        }

        bool operator>(const ColoredPP& other) const{
            if(_type == other._type) return _power > other._power;
            return _type > other._type;
        }

        const std::string& getType()const{
            return _type;
        }
        size_t getPower() const{
            return _power;
        }
        void setPower(size_t power){
            this->_power = power;
        }
    private:
        std::string _type;
        size_t _power;
    };
}