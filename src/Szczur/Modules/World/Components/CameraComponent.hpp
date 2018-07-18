#pragma once

#include <SFML/Graphics.hpp>
#include <nlohmann/json_fwd.hpp>

#include "Szczur/Utility/SFML3D/Camera.hpp"

#include "../Component.hpp"


namespace rat {

class InputManager;
class Entity;
class Script;
class Listener;
template<class T> class ScriptClass;

class CameraComponent : public Component, public sf3d::Camera {
public:

    ///
    CameraComponent(Entity* parent);

    ///
    virtual std::unique_ptr<Component> copy(Entity* newParent) const override;

    ///
    virtual void loadFromConfig(nlohmann::json& config) override;

    ///
    virtual void saveToConfig(nlohmann::json& config) const override;

    ///
    void setVelocity(float velocity);

    ///
    float getVelocity() const;

    void setSmoothness(float smoothness);

    float getSmoothness() const;

    void stickTo(Entity* entity);

	void stickTo(float x);

    ///
    void setLock(bool lock);

    ///
    bool getLock() const;

    ///
    bool isNoMove() const;

    ///
    void setNoMove(bool flag);

    ///
    virtual void renderHeader(ScenesManager& scenes, Entity* object) override;
    
    void stickToPlayer();

    ///
    void update(ScenesManager& scenes, float deltaTime);

    /// 
    void updateCamera();

    ///
    static void initScript(ScriptClass<Entity>& entity, Script& script);

private:
    bool _locked{false};
    bool _noMove = false;
	float _velocity{50.f};

	glm::vec3 _virtualPosition{0.f};

    bool _limitedRange{false};
    struct {
        float left{0.f};
        float right{0.f};
    } _limit;


	union {
		float _smoothness{1.f};
		float _linear;
	};

	enum MoveType : size_t {
		None,
		Smooth,
		Linear
	} _moveType{None};


	union {
		float _forwarded{1.f};
	};

	enum TargetingType : size_t {
		Precise,
		Forwarded
	} _targetingType{Precise};

	static std::string enumToString(MoveType type);
	static std::string enumToString(TargetingType type);

    Entity* _stickTo{nullptr};
    int _stickedID = 0;

	float _stickToX{0.f};
	bool _stickedToX{false};

    Listener* _listener;

};

}