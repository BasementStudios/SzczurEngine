#pragma once

#include <nlohmann/json_fwd.hpp>

#include "Szczur/Utility/SFML3D/RenderTarget.hpp"
#include "Szczur/Utility/SFML3D/RenderStates.hpp"

#include "Szczur/Modules/World/Component.hpp"
#include "Szczur/Modules/World/Data/SpriteDisplayData.hpp"

namespace rat
{

class Entity;
class Script;
template<class T> class ScriptClass;

class BaseComponent : public Component
{
public:

	///
	BaseComponent(Entity* parent);

	///
	BaseComponent(const BaseComponent&) = default;

	///
	BaseComponent& operator = (const BaseComponent&) = default;

	///
	BaseComponent(BaseComponent&&) = default;

	///
	BaseComponent& operator = (BaseComponent&&) = default;

	///
	~BaseComponent() = default;

	///
	virtual std::unique_ptr<Component> copy(Entity* newParent) const override;

	///
	virtual void loadFromConfig(nlohmann::json& config) override;

	///
	virtual void saveToConfig(nlohmann::json& config) const override;

	///
	virtual void renderHeader(ScenesManager& scenes, Entity* object) override;

	///
	void positionOnly(bool flag);

	///
	static void initScript(ScriptClass<Entity>& entity, Script& script);

private:

	bool _positionOnly = false;

};

}
