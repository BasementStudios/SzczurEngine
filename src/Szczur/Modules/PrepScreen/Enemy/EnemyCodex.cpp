#include "EnemyCodex.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"

namespace rat
{
    EnemyCodex::EnemyCodex()
    {
        _testInit();
    }

    void EnemyCodex::initAssetsViaGUI(GUI& gui)
    {
        for(auto& [name, enemy] : _enemies)
        {
            auto& path = enemy->getIconPath();
            gui.addAsset<sf::Texture>(path);
            enemy->setTexture(gui.getAsset<sf::Texture>(path));
        }
    }

    void EnemyCodex::_testInit()
    {
        _addEnemy("Stefan", "Assets/Test/Enemy1.png", 10);
        _addEnemy("Maciek", "Assets/Test/Enemy2.png", 13);
        _addEnemy("PsychoX", "Assets/Test/Enemy3.png", 11);
        _addEnemy("Hefa", "Assets/Test/Enemy4.png", 3);
    }
    void EnemyCodex::_addEnemy(const std::string& name, const std::string& path, int hp)
    {
        auto enemy = std::make_unique<Enemy>();
        enemy->setName(name);
        enemy->setIconPath(path);
        enemy->setMaxHP(hp);
        _enemies.emplace(name, std::move(enemy));
    }
    
}