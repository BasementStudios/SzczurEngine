#include "Requirements.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    void Requirements::addCounter(const std::string& name, int maxValue)
    {
        auto found = _counters.find(name);
        if(found != _counters.end())
        {
            LOG_INFO("Counter \"", name, "\" already exist.");
            return;
        }
        _counters.emplace(name, Counter{0, maxValue});
    }
    void Requirements::advanceCounter(const std::string& name, int value)
    {
        auto found = _counters.find(name);
        if(found == _counters.end())
        {
            LOG_INFO("Counter \"", name, "\" doesn't exists.");
            return;
        }
        bool wasFull = found->second.isFull();
        found->second.current += value;
        if(!wasFull && found->second.isFull()) ++_fullCountersAmount;
        if(wasFull && !found->second.isFull()) --_fullCountersAmount;
    }
    int Requirements::getValueFromCounter(const std::string& name) const
    {
        auto found = _counters.find(name);
        if(found == _counters.end())
        {
            LOG_INFO("Counter \"", name, "\" doesn't exist.");
            return 0;
        }
        return found->second.current;
    }
    bool Requirements::isCounterFull(const std::string& name)
    {
        auto found = _counters.find(name);
        if(found == _counters.end())
        {
            LOG_INFO("Counter \"", name, "\" doesn't exist.");
            return 0;
        }
        return found->second.isFull();
    }

    bool Requirements::areAllCountersFull() const
    {
        return _fullCountersAmount == _counters.size();
    }

    void Requirements::addReq(const std::string& name)
    {
        auto found = _reqs.find(name);
        if(found != _reqs.end())
        {
            LOG_INFO("Requirement \"", name, "\" already exists.");
            return;
        }
        _reqs.emplace(name, false);
    }
    bool Requirements::isReqSuited(const std::string& name) const
    {
        auto found = _reqs.find(name);
        if(found == _reqs.end())
        {
            LOG_INFO("Requirement \"", name, "\" doesn't exist.");
            return false;
        }
        return found->second;
    }
    void Requirements::suitReq(const std::string& name)
    {
        auto found = _reqs.find(name);
        if(found == _reqs.end())
        {
            LOG_INFO("Requirement \"", name, "\" doesn't exist.");
            return;
        }
        bool wasSuited = found->second;
        found->second = true;
        if(!wasSuited && found->second) ++_suitedReqsAmount;
    }
    bool Requirements::areAllReqsSuited() const
    {
        return _suitedReqsAmount == _reqs.size();
    }

    bool Requirements::isFullySuited() const
    {
        return (areAllCountersFull() && areAllReqsSuited());
    }
    
}