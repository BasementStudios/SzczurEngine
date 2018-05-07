
//#ifdef EDITOR
#pragma once

#include "Entity.hpp"
#include "Scene.hpp"
#include "Components/ArmatureComponent.hpp"
#include "Components/SpriteComponent.hpp"
#include "data/SpriteDisplayData.hpp"
#include "data/ArmatureDisplayData.hpp"

#include <boost/container/flat_map.hpp>
#include "Szczur/Modules/Input/InputManager.hpp"
#include "Szczur/Modules/Camera/Camera.hpp"

namespace rat {
    class LevelEditor {
    public:
        LevelEditor();

        void setScene(Scene* scene, size_t camera);

        void render(sf3d::RenderTarget& target);
        void update(InputManager& input, Camera& camera);
    private:
        Scene* _scene{nullptr};

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
        size_t _focusedObject{static_cast<size_t>(-1)};
        size_t _camera{static_cast<size_t>(-1)};
        struct {
            glm::vec3 position{0.f, 0.f, 0.f};
            glm::vec3 rotation{0.f, 0.f, 0.f};
            void move(const glm::vec3& offset) {position += offset;}
            void rotate(const glm::vec3& offset) {rotation += offset;}
        } _freeCamera;
        float _freeCameraVelocity{50.f};
        bool _rotatingCamera{false};
        sf::Vector2i _previousMouse;
    };
}

//#endif