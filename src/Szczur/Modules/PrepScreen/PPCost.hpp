#pragma once

#include <map>
#include <algorithm>
#include <initializer_list>

#include "Szczur/Modules/PrepScreen/PPContainer.hpp"

namespace rat
{

class PPCost
{
public:
    bool isBuyable(const PPContainer& source)
    {
        for(const auto& [type, cost] : costs)
        {
            if(cost == 0) continue;
            size_t price = cost;
            for(size_t i = 2; i >= 0; i--)
            {
                size_t payment = std::min(price/i, source.getAmount(type, i)) * i;
                price -= payment;
            }
            if(price > 0) return false;
        }
        return true;
    }

    void buyFrom(PPContainer& source)
    {
        assert(isBuyable(source));
        for(const auto& [type, cost] : costs)
        {
            if(cost == 0) continue;
            size_t price = cost;
            for(size_t i = 2; i > 0; i--)
            {
                size_t numOfPP = std::min(price/i, source.getAmount(type, i));
                price -= numOfPP * i;
                PPGroup payment(type, i, numOfPP);
                source.remove(payment);
                paymentCont.add(payment);
            }
        }
    }

    void returnPaymentTo(PPContainer& source)
    {
        paymentCont.moveAllTo(source);
    }

    void setCost(const std::string type, size_t price)
    {
        assert(!paymentCont.hasAnyPP());
        costs[type] = price;
    }

    void setCosts(std::initializer_list<std::pair<std::string, size_t>> l)
    {
        costs.clear();
        for(auto& cost : l)
        {
            setCost(cost.first, cost.second);
        }
    }

    PPCost() = default;

    PPCost& operator= (std::initializer_list<std::pair<std::string, size_t>> l)
    {
        setCosts(l);
        return *this;
    }
    PPCost (std::initializer_list<std::pair<std::string, size_t>> l)
    {
        setCosts(l);
    }

private:
    std::map<std::string, size_t> costs;
    PPContainer paymentCont;
};

}