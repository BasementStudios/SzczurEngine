#pragma once

#include <unordered_map>
#include <string>
#include <fstream>

#include <Json/json.hpp>

namespace rat
{
    class Requirements
    {
    public:
        void addCounter(const std::string& name, int maxValue);
        virtual void advanceCounter(const std::string& name, int value = 1);
        int getValueFromCounter(const std::string& name) const;
        bool isCounterFull(const std::string& name);

        bool areAllCountersFull() const;

        void addReq(const std::string& name);
        bool isReqSuited(const std::string& name) const;
        virtual void suitReq(const std::string& name);

        bool areAllReqsSuited() const;

        bool isFullySuited() const;

        void resetValues();

        nlohmann::json getJson() const;
        void loadFromJson(nlohmann::json& j);

    private:
        struct Counter 
        {
            int current = 0; 
            int maximum = 0; 
            bool isFull() const { return current >= maximum; }
        };
        std::unordered_map<std::string, Counter> _counters;
        std::unordered_map<std::string, bool> _reqs;

        size_t _fullCountersAmount = 0;
        size_t _suitedReqsAmount = 0;
    };
}