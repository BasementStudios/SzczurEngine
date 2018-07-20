#pragma once

#include <SFML/Graphics/Texture.hpp>

#include <Json/json.hpp>

namespace rat
{
    class Enemy
    {
    public:
        Enemy() = default;
        Enemy(nlohmann::json& j);

        void setName(const std::string& name);
        const std::string& getName() const;

        void setIconPath(const std::string& path);
        const std::string& getIconPath() const;

        void setMaxHP(int hp);
        int getMaxHP() const;

        const std::string& getDescription() const { return _desc; }

        void setTexture(sf::Texture* tex);
        sf::Texture* getTexture() const;
    private:
        std::string _name{""};
        std::string _iconPath{""};
        int _maxHP{0};
        sf::Texture* _iconTex{nullptr};
        std::string _desc;
    };
}