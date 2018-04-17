#pragma once
#include <array>
#include <string>

namespace rat
{
    class PPColors
    {
    public:
    PPColors();
    auto begin() const
    {
        return _colors.begin();
    }
    auto end() const
    {
        return _colors.end();
    }
    private:
        const std::array<std::string, 7u> _colors;
    };
}