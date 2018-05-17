#pragma once

#include <SFML/Graphics/Texture.hpp>

namespace rat
{
    class Enemy
    {
    public:
        void setName(const std::string& name);
        const std::string& getName() const;

        void setIconPath(const std::string& path);
        const std::string& getIconPath() const;

        void setMaxHP(int hp);
        int getMaxHP() const;

        void setTexture(sf::Texture* tex);
        sf::Texture* getTexture() const;
    private:
        std::string _name{""};
        std::string _iconPath{""};
        int _maxHP{0};
        sf::Texture* _iconTex{nullptr};
    };
}