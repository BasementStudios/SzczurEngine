#pragma once

/** @file PointLightComponent.hpp
 ** @description
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <memory> // unique_ptr
#include <variant>

#include <glm/vec3.hpp>
#include <Json/json.hpp>
using Json = nlohmann::json;

namespace rat {
	class ScenesManager;
	class Entity;
	class Script;
	template<class T> class ScriptClass;
}

#include "Szczur/Utility/SFML3D/LightPoint.hpp"
#include "Szczur/Modules/World/Component.hpp"

namespace rat
{

/** @class PointLightComponent
 ** @description Component which store and manipulate data related to point light used on the scenes.
 **/
class PointLightComponent : public Component, public sf3d::LightPoint
{
	/* Operators */
public:
	PointLightComponent(Entity* parent);



	/* Methods */
public:
	// Copy component
    virtual std::unique_ptr<Component> copy(Entity* newParent) const override;

	// Configuration
    virtual void loadFromConfig(Json& config) override;
    virtual void saveToConfig(Json& config) const override;

	// Editor view
    virtual void renderHeader(ScenesManager& scenes, Entity* object) override;

	// Scripting
	static void initScript(ScriptClass<Entity>& entity, Script& script);
};

}
