#pragma once
#include <set>
#include <string>

namespace rat
{
    class PPColors
    {
    public:
    PPColors();
    auto begin()
    {
        return _colors.begin();
    }
    auto end()
    {
        return _colors.end();
    }
    private:
        static std::set<std::string> _staticColors;
        std::set<std::string>& _colors;
    };
}