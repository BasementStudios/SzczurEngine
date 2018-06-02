#include "Requirements.hpp"

#include "Szczur/Utility/Logger.hpp"

using namespace nlohmann;

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

    json Requirements::getJson() const
    {
        json j;

        json counters;
        for(auto& [name, count] : _counters)
        {
            counters[name] = json::array({count.current, count.maximum});
        }

        json reqs(_reqs);

        j.emplace_back(std::move(counters));
        j.emplace_back(std::move(reqs));

        return j;
    }

    void Requirements::loadFromJson(nlohmann::json& j)
    {
        auto& counters = j[0];
        auto& reqs = j[1];

        
        _counters.reserve(counters.size());
        for(auto i = counters.begin(); i != counters.end(); i++)
        {
            auto& value = i.value();
            _counters[i.key()] = {value[0], value[1]};
        }

        _reqs.reserve(reqs.size());
        for(auto i = reqs.begin(); i != reqs.end(); i++)
        {
            _reqs[i.key()] = i.value();
        }
    }

    void Requirements::resetValues()
    {
        for(auto& [name, req] : _reqs)
        {
            req = false;
        }
        for(auto& [name, counter] : _counters)
        {
            std::cout << name << " reseted\n";
            counter.current = 0;
        }
    }

    
    void Requirements::initScript(Script& script) 
    {
            auto object = script.newClass<Requirements>("Requirements", "QuestLog");

            // Main
            object.set("addCounter", &Requirements::addCounter);
            object.set("getValueFromCounter", &Requirements::getValueFromCounter);
            object.set("addReq", &Requirements::addReq);
            object.set("isReqSuited", &Requirements::isReqSuited);
            object.set("suitReq", &Requirements::suitReq);
            object.set("isFullySuited", &Requirements::isFullySuited);

            object.init();
        }
    
}