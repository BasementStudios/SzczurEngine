#include "PointLightComponent.hpp"

/** @file PointLightComponent.cpp
 ** @description
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <nlohmann/json.hpp>
using Json = nlohmann::json;
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Utility/Convert/Hash.hpp" // fnv1a_64
#include "Szczur/Modules/World/Entity.hpp"
#include "Szczur/Modules/World/Component.hpp"

namespace rat
{

/* Operators */
PointLightComponent::PointLightComponent(Entity* parent)
: 	Component {parent, fnv1a_64("PointLightComponent"), "PointLightComponent"}
{
	;
}



/* Methods */
// Copy component
std::unique_ptr<Component> PointLightComponent::copy(Entity* newParent) const {
	auto ptr = std::make_unique<PointLightComponent>(*this);

	ptr->setEntity(newParent);

	return ptr;
}

// Configuration
void PointLightComponent::loadFromConfig(Json& config) {
	Component::loadFromConfig(config);
	
	// Color
	if (auto& subconfig = config["color"]; !subconfig.is_null()) {
		if (auto& var = subconfig["r"]; !var.is_null()) this->color.r = var;
		if (auto& var = subconfig["g"]; !var.is_null()) this->color.g = var;
		if (auto& var = subconfig["b"]; !var.is_null()) this->color.b = var;
	}

	// Attenuation
	if (auto& subconfig = config["attenuation"]; !subconfig.is_null()) {
		if (auto& var = subconfig["constant"];  !var.is_null()) this->attenuation.constant  = var;
		if (auto& var = subconfig["linear"];    !var.is_null()) this->attenuation.linear    = var;
		if (auto& var = subconfig["quadratic"]; !var.is_null()) this->attenuation.quadratic = var;
	}

	// Light factors
	if (auto& subconfig = config["ambientFactor"]; !subconfig.is_null()) {
		if (auto& var = subconfig["r"]; !var.is_null()) this->ambientFactor.r = var;
		if (auto& var = subconfig["g"]; !var.is_null()) this->ambientFactor.g = var;
		if (auto& var = subconfig["b"]; !var.is_null()) this->ambientFactor.b = var;
	}
	if (auto& subconfig = config["diffuseFactor"]; !subconfig.is_null()) {
		if (auto& var = subconfig["r"]; !var.is_null()) this->diffuseFactor.r = var;
		if (auto& var = subconfig["g"]; !var.is_null()) this->diffuseFactor.g = var;
		if (auto& var = subconfig["b"]; !var.is_null()) this->diffuseFactor.b = var;
	}
	if (auto& subconfig = config["specularFactor"]; !subconfig.is_null()) {
		if (auto& var = subconfig["r"]; !var.is_null()) this->specularFactor.r = var;
		if (auto& var = subconfig["g"]; !var.is_null()) this->specularFactor.g = var;
		if (auto& var = subconfig["b"]; !var.is_null()) this->specularFactor.b = var;
	}
}
void PointLightComponent::saveToConfig(Json& config) const {
	Component::saveToConfig(config);
	
	// Color
	config["color"]["r"] = this->color.r;
	config["color"]["g"] = this->color.g;
	config["color"]["b"] = this->color.b;

	// Attenuation
	config["attenuation"]["constant"] 	= this->attenuation.constant;
	config["attenuation"]["linear"] 	= this->attenuation.linear;
	config["attenuation"]["quadratic"] 	= this->attenuation.quadratic;

	// Light factors
	config["ambientFactor"]["r"]	= this->ambientFactor.r;
	config["ambientFactor"]["g"]	= this->ambientFactor.g;
	config["ambientFactor"]["b"]	= this->ambientFactor.b;
	config["diffuseFactor"]["r"]	= this->diffuseFactor.r;
	config["diffuseFactor"]["g"]	= this->diffuseFactor.g;
	config["diffuseFactor"]["b"]	= this->diffuseFactor.b;
	config["specularFactor"]["r"]	= this->specularFactor.r;
	config["specularFactor"]["g"]	= this->specularFactor.g;
	config["specularFactor"]["b"]	= this->specularFactor.b;
}

// Editor view
void PointLightComponent::renderHeader(ScenesManager& scenes, Entity* object) {
	if (ImGui::CollapsingHeader("Light point##lightpoint_component")) {
		// Color
		ImGui::ColorEdit3("Light color##lightpoint_component", glm::value_ptr(this->color));

		// Attenuation
		ImGui::DragFloat("Attenuation constant##lightpoint_component",	&this->attenuation.constant,	0.005f,		-1.0f,	1.0f, "%.7f");
		ImGui::DragFloat("Attenuation linear##lightpoint_component", 	&this->attenuation.linear,		0.00005f,	-1.0f,	1.0f, "%.7f");
		ImGui::DragFloat("Attenuation quadratic##lightpoint_component", &this->attenuation.quadratic,	0.0000005f, -1.0f,	1.0f, "%.7f");

		// Light factors
		ImGui::ColorEdit3("Ambient factor##base_component",	glm::value_ptr(this->ambientFactor));
		ImGui::ColorEdit3("Diffuse factor##base_component",	glm::value_ptr(this->diffuseFactor));
		ImGui::ColorEdit3("Specular factor##base_component",glm::value_ptr(this->specularFactor));
	}
}

// Scripting
void PointLightComponent::initScript(ScriptClass<Entity>& entity, Script& script)
{
	auto object = script.newClass<PointLightComponent>("PointLightComponent", "World");

	// Component
	{
		// Color
		object.set("getColor", &PointLightComponent::getColor);
		object.set("setColor", [](PointLightComponent& component, float r, float g, float b) {component.setColor({r, g, b});});

		// Attenuation
		object.set("getAttenuation", &PointLightComponent::getAttenuation);
		object.set("setAttenuation", [](PointLightComponent& component, float c, float l, float q) {component.setAttenuation(LightPoint::Attenuation{c, l, q});});
		
		// Light factors
		object.set("getAmbientFactor", &PointLightComponent::getAmbientFactor);
		object.set("setAmbientFactor", [](PointLightComponent& component, float r, float g, float b) {component.setAmbientFactor({r, g, b});});
		object.set("getDiffuseFactor", &PointLightComponent::getDiffuseFactor);
		object.set("setDiffuseFactor", [](PointLightComponent& component, float r, float g, float b) {component.setDiffuseFactor({r, g, b});});
		object.set("getSpecularFactor", &PointLightComponent::getSpecularFactor);
		object.set("setSpecularFactor", [](PointLightComponent& component, float r, float g, float b) {component.setSpecularFactor({r, g, b});});
	}
	object.set("getEntity", sol::resolve<Entity*()>(&Component::getEntity));

	// Entity
	entity.set("addPointLightComponent", [&](Entity& e){return (PointLightComponent*)e.addComponent<PointLightComponent>();});
	entity.set("pointLight", &Entity::getComponentAs<PointLightComponent>);

	object.init();
}

}
