#pragma once

#include "SpriteComponent.hpp"
#include "ArmatureComponent.hpp"
#include "InputControllerComponent.hpp"
#include "Szczur/Utility/SFML3D/Drawable.hpp"

namespace rat
{

struct ComponentTraits
{
	///
	static std::unique_ptr<Component> createFromComponentID(size_t componentID)
	{
		if (componentID == typeID<SpriteComponent>()) return std::make_unique<SpriteComponent>();
		if (componentID == typeID<ArmatureComponent>()) return std::make_unique<ArmatureComponent>();
		if (componentID == typeID<InputControllerComponent>()) return std::make_unique<InputControllerComponent>();
		return nullptr;
	}

	///
	static std::unique_ptr<Component> createFromName(const std::string& name)
	{
		if (name == "SpriteComponent") return std::make_unique<SpriteComponent>();
		if (name == "ArmatureComponent") return std::make_unique<ArmatureComponent>();
		if (name == "InputControllerComponent") return std::make_unique<InputControllerComponent>();

		return nullptr;
	}

	///
	template <typename T>
	static std::unique_ptr<Component> createFromType()
	{
		return std::make_unique<T>();
	}

	///
	static size_t getIdentifierFromName(const std::string& name)
	{
		if (name == "SpriteComponent") return typeID<SpriteComponent>();
		if (name == "ArmatureComponent") return typeID<ArmatureComponent>();
		if (name == "Drawable") return typeID<sf3d::Drawable>();
		if (name == "InputControllerComponent") return typeID<InputControllerComponent>();

		return 0;
	}

	///
	static std::string getNameFromIdentifier(size_t id)
	{
		if (id == typeID<SpriteComponent>()) return "SpriteComponent";
		if (id == typeID<ArmatureComponent>()) return "ArmatureComponent";
		if (id == typeID<sf3d::Drawable>()) return "Drawable";
		if (id == typeID<InputControllerComponent>()) return "InputControllerComponent";

		return "";
	}

};

}
