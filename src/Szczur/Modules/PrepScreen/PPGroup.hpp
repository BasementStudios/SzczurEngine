#pragma once

#include <string>
#include <cassert>

namespace rat
{
class PPGroup
{
public:
    PPGroup(const std::string& type, size_t power = 1u, size_t amount = 1u)
    :
    power(power),
    amount(amount),
    type(type)
    {
    }

    void add(size_t addon)
    {
        amount += addon;
    }
    void add(const PPGroup& other)
    {
        assert(other.power == power && other.type == type);
        add(other.amount);
    }
    void remove(size_t sub)
    {
        assert(amount >= sub);
        amount -= sub;
    }
    void remove(const PPGroup& other)
    {
        assert(other.power == power && other.type == type);
        remove(other.amount);
    }

    size_t getPower() const{
        return power;
    }
    size_t getAmount() const{
        return amount;
    }
    const std::string& getType() const{
        return type;
    }

    bool operator>(const PPGroup& other) const{
        if(type == other.type) return power > other.power;
        return type > other.type;
    }



private:

    const size_t power;
    size_t amount;
    const std::string type;
};
}