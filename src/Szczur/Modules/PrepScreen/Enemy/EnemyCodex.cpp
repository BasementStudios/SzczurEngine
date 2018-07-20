#include "EnemyCodex.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"

namespace rat
{
    EnemyCodex::EnemyCodex()
    {
       
    }

    void EnemyCodex::initAssetsViaGUI(GUI& gui)
    {
        for(auto& [name, enemy] : _enemies)
        {
            auto& path = enemy->getIconPath();
            gui.addAsset<sf::Texture>(_iconPaths + path);
            enemy->setTexture(gui.getAsset<sf::Texture>(_iconPaths + path));
        }
    }

    void EnemyCodex::loadFromJson(nlohmann::json& j)
    {
        _enemies.clear();

        if(j.find("iconsPath") != j.end())
        {
            _iconPaths = j["iconsPath"];
        }

        nlohmann::json ps = j["pawns"];
        for(auto it = ps.begin(); it != ps.end(); ++it)
        {
            nlohmann::json pawn = it.value();

            if(!(pawn["isEnemy"].get<bool>())) continue;
            _enemies.emplace(it.key(), std::make_unique<Enemy>(pawn));
        }
    }

    const Enemy* EnemyCodex::getEnemy(const std::string& nameID)
    {
        auto found = _enemies.find(nameID);
        if(found == _enemies.end()) return nullptr;
        return found->second.get();
    }
    
}