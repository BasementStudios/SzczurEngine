#pragma once

#include "PPcost.hpp"

namespace rat
{
    class Skill
    {
        public:
        
        void setName(const std::string& newName)
        {
            _name = newName;
        }
        const std::string& getName() const {
            return _name;
        }
        void setTexturePath(const std::string& newName)
        {
            _texturePath = newName;
        }
        const std::string& getTexturePath() const {
            return _texturePath;
        }

        private:
        std::string _name;
        std::string _texturePath;
        PPCost _cost;
    };
}