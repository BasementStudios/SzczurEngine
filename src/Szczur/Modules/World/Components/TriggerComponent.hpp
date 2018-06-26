#pragma once
#include <Sol/sol.hpp>

#include <glm/glm.hpp>

#include "../Component.hpp"

namespace rat {

class InputManager;
class Entity;
class Script;
template<class T> class ScriptClass;

class TriggerComponent : public Component {
public:

	///
	enum Type : size_t { 
		None = 0u, 
		ChangeScene = 1u, 
		Overlaping = 2u,
		TypesCount 
	} type{None};

	///
	union {
		struct{ size_t sceneId; size_t entranceId; };
	};

	///
	TriggerComponent(Entity* parent);

	///
	bool checkForTrigger(const glm::vec3& position) const;

	///
	virtual std::unique_ptr<Component> copy(Entity* newParent) const override;

	///
	virtual void loadFromConfig(Json& config) override;

	///
	virtual void saveToConfig(Json& config) const override;

	///
	void setRadius(float radius);

	///
	float getRadius() const;

	///
	static std::string enumToString(size_t en);

	///
	static Type uintToEnum(size_t s);

	///
	virtual void renderHeader(ScenesManager& scenes, Entity* object) override;

    ///
    void update(ScenesManager& scenes, float deltaTime);

	///
	static void initScript(ScriptClass<Entity>& entity, Script& script);

private:
	InputManager& _input;
	float _radius{10.f};

	bool _changingSceneWithFade = true;
	float _fadeTime = 1.f;

	bool _isPlayerInside = false;
	sol::function _enterCallback;
	sol::function _insideCallback;
	sol::function _leaveCallback;

};

}
