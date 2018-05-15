#include "PPContainer.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    size_t PPContainer::getAmount() const
    {
        return _actualAmount;
    }
    size_t PPContainer::getTotalAmount() const
    {
        _totalAmount;
    }
    
    void PPContainer::add(amount_t addon)
    {
        _actualAmount += addon;
        _totalAmount += addon;
    }
    void PPContainer::remove(amount_t sub)
    {
        if(_totalAmount < sub)
        {
            LOG_ERROR(this, "Trying to remove not existing pps.");
            return;
        }
        if(_actualAmount < sub)
        {
            LOG_ERROR(this, "Trying to remove taken pps.");
            return;        
        }
        _totalAmount -= sub;
        _actualAmount -= sub;
    }

    void PPContainer::takeFrom(amount_t takenAmount)
    {
        if(_actualAmount < takenAmount)
        {
            LOG_ERROR(this, "Trying to take not existing pps..");
            return;        
        }
        _actualAmount -= takenAmount;        
    }
    void PPContainer::returnTo(amount_t returnAmount)
    {
        if(_totalAmount - _actualAmount < returnAmount)
        {
            LOG_ERROR(this, "Trying to return pps redundantly.");
            return;
        }
        _actualAmount += returnAmount;
    }

    void PPContainer::reset()
    {
        _totalAmount = _actualAmount;
    }
}