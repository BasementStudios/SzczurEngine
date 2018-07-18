#pragma once

#include <algorithm>

#include "Szczur/Utility/Logger.hpp"


namespace rat
{
    class PPContainer
    {
    public:
        void setMaximumSlotsAmount(int amount)
        {
            _maximum = amount;
            _unlocked = std::min(_maximum, _unlocked);
            _filled = std::min(_filled, _unlocked);
        }
        int getMaximumSlotsAmount() const
        {
            return _maximum;
        }

        void unlockSlots(int addon = 1)
        {
            setUnlockedAmount(_unlocked + addon);
        }
        void lockSlots(int sub = 1)
        {
            setUnlockedAmount(_unlocked - sub);
        }
        void setUnlockedAmount(int amount)
        {
            if(amount > _maximum || amount < 0)
            {
                LOG_ERROR("TODO");
                return;
            }
            _unlocked = amount;
        }
        int getUnlockedSlotsAmount() const
        {
            return _unlocked;
        }
        int getLockedSlotsAmount() const
        {
            return _maximum - _unlocked;
        }

        void fillPP(int amount)
        {
            setFilledPPAmount(_filled + amount);
        }
        void emptyPP(int amount)
        {
            setFilledPPAmount(_filled - amount);
        }
        void setFilledPPAmount(int amount)
        {
            if(amount < 0 || amount > _unlocked)
            {
                LOG_ERROR("TODO");
                return;
            }
            _filled = amount;
        }
        int getFilledPPAmount() const
        {
            return _filled;
        }
        int getEmptyPPAmount() const
        {
            return _unlocked - _filled;
        }

        void reset();
    private:
        int _filled{0};
        int _unlocked{0};
        int _maximum{0};
    };
}