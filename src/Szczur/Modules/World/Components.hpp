#pragma once

#include <Szczur/Modules/Script/Script.hpp>

#include "Components/SpriteComponent.hpp"
#include "Components/ArmatureComponent.hpp"
#include "Components/CameraComponent.hpp"
#include "Components/ScriptableComponent.hpp"

#include <memory>

namespace rat
{

// FWD
class Entity;

struct ComponentTraits
{
	///
	static std::unique_ptr<Component> createFromComponentID(Entity* parent, Hash64_t id)
	{
		if (id == fnv1a_64("SpriteComponent")) return std::make_unique<SpriteComponent>(parent);
		if (id == fnv1a_64("ArmatureComponent")) return std::make_unique<ArmatureComponent>(parent);
		if (id == fnv1a_64("CameraComponent")) return std::make_unique<CameraComponent>(parent);
		if (id == fnv1a_64("ScriptableComponent")) return std::make_unique<ScriptableComponent>(parent);

		return nullptr;
	}

	///
	static std::unique_ptr<Component> createFromName(Entity* parent, const std::string& name)
	{
		if (name == "SpriteComponent") return std::make_unique<SpriteComponent>(parent);
		if (name == "ArmatureComponent") return std::make_unique<ArmatureComponent>(parent);
		if (name == "CameraComponent") return std::make_unique<CameraComponent>(parent);
		if (name == "ScriptableComponent") return std::make_unique<ScriptableComponent>(parent);

		return nullptr;
	}

	///
	template <typename T>
	static std::unique_ptr<Component> createFromType(Entity* parent)
	{
		return std::make_unique<T>(parent);
	}

	///
	static Hash64_t getIdentifierFromName(const std::string& name)
	{
		if (name == "SpriteComponent") return fnv1a_64("SpriteComponent");
		if (name == "ArmatureComponent") return fnv1a_64("ArmatureComponent");
		if (name == "CameraComponent") return fnv1a_64("CameraComponent");
		if (name == "ScriptableComponent") return fnv1a_64("ScriptableComponent");

		return 0;
	}

	///
	static std::string getNameFromIdentifier(Hash64_t id)
	{
		if (id == fnv1a_64("SpriteComponent")) return "SpriteComponent";
		if (id == fnv1a_64("ArmatureComponent")) return "ArmatureComponent";
		if (id == fnv1a_64("CameraComponent")) return "CameraComponent";
		if (id == fnv1a_64("ScriptableComponent")) return "ScriptableComponent";

		return "";
	}

	///
	template <typename T>
	static Hash64_t getIdentifierFromType()
	{
		if (std::is_same_v<T, SpriteComponent>) return fnv1a_64("SpriteComponent");
		if (std::is_same_v<T, ArmatureComponent>) return fnv1a_64("ArmatureComponent");
		if (std::is_same_v<T, CameraComponent>) return fnv1a_64("CameraComponent");
		if (std::is_same_v<T, ScriptableComponent>) return fnv1a_64("ScriptableComponent");

		return 0;
	}

	///
	template <typename T>
	static Component::Feature_e getFeatureFromType()
	{
		if (std::is_same_v<T, sf3d::Drawable>) return Component::Drawable;

		return static_cast<Component::Feature_e>(0);
	}

	static void initScript(Script& script) {
		auto module = script.newModule("World");
		script.initClasses<ScriptableComponent>();
	}
};

}
