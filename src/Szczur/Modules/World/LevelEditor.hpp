
//#ifdef EDITOR
#pragma once

#include "Entity.hpp"
#include "SpriteDisplayData.hpp"
#include "ArmatureComponent.hpp"
#include "ArmatureDisplayData.hpp"

#include <boost/container/flat_map.hpp>
#include "Szczur/Modules/Input/InputManager.hpp"
#include "Szczur/Modules/Camera/Camera.hpp"

namespace rat {
    class LevelEditor {
    public:
        using EntitiesHolder_t   = std::vector<Entity>;
		using SpriteDisplayDataHolder_t = std::vector<SpriteDisplayData>;
		using ArmatureDisplayDataHolder_t = std::vector<ArmatureDisplayData>;
		using CollectingHolder_t = boost::container::flat_map<std::string, EntitiesHolder_t>;

        LevelEditor(
            CollectingHolder_t &collectingHolder,
            SpriteDisplayDataHolder_t &spriteDisplayDataHolder,
            ArmatureDisplayDataHolder_t &armatureDisplayDataHolder
        );
        void render(sf3d::RenderTarget& target);
        void update(InputManager& input, Camera& camera);
    private:
        CollectingHolder_t& _collectingHolder;

		SpriteDisplayDataHolder_t& _spriteDisplayDataHolder;

		ArmatureDisplayDataHolder_t& _armatureDisplayDataHolder;
        
        EntitiesHolder_t::iterator addEntity(const std::string group, const std::string name = _getUniqueName());
        Entity* getEntity(const std::string group, const std::string name);

		const Entity* getEntity(const std::string group, const std::string name) const;
		bool removeEntity(const std::string group, const std::string name);
		void removeAllEntities(const std::string group);
		void removeAllEntities();
		bool hasEntity(const std::string group, const std::string name);
		void loadFromFile(const std::string& filepath);
		void saveToFile(const std::string& filepath) const;

        void _renderBar();
        void _renderDisplayDataManager();
        void _renderArmatureDisplayManager();
        void _renderFocusedObjectsParams();
        void _renderObjectsList();
        void _renderModulesManager();

        bool _ifRenderObjectsList{true};
        bool _ifRenderDisplayDataManager{false};
        bool _ifRenderArmatureDisplayManager{false};
        bool _anySelected{false};
        bool _ifRenderModulesManager{false};
        EntitiesHolder_t::iterator _focusedObject;
        EntitiesHolder_t::iterator _camera;
        Entity _freeCamera{""};
        float _freeCameraVelocity{50.f};
        bool _rotatingCamera{false};
        sf::Vector2i _previousMouse;

        static std::string _getUniqueName();
        EntitiesHolder_t& _getSubHolder(const std::string& group);

		const EntitiesHolder_t& _getSubHolder(const std::string& group) const;

		typename EntitiesHolder_t::iterator _find(const std::string group, const std::string& name);

		typename EntitiesHolder_t::const_iterator _find(const std::string group, const std::string& name) const;
    };
}

//#endif