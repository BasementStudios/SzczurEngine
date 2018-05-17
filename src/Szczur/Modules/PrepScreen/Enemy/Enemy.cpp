#include "Enemy.hpp"

namespace rat
{
    void Enemy::setName(const std::string& name)
    {
        _name = name;
    }
    const std::string& Enemy::getName() const
    {
        return _name;
    }

    void Enemy::setIconPath(const std::string& path)
    {
        _iconPath = path;
    }
    const std::string& Enemy::getIconPath() const
    {
        return _iconPath;
    }

    void Enemy::setMaxHP(int hp)
    {
        _maxHP = hp;
    }
    int Enemy::getMaxHP() const
    {
        return _maxHP;
    }

    void Enemy::setTexture(sf::Texture* tex)
    {
        _iconTex = tex;
    }
    sf::Texture* Enemy::getTexture() const
    {
        return _iconTex;
    }
}