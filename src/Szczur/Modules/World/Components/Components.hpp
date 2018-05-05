#pragma once

#include "SpriteComponent.hpp"

namespace rat
{

struct ComponentTraits
{
	///
	static std::unique_ptr<Component> createFromComponentID(size_t componentID)
	{
		if (componentID == typeID<SpriteComponent>()) return std::make_unique<SpriteComponent>();

		return nullptr;
	}

	///
	static std::unique_ptr<Component> createFromName(const std::string& name)
	{
		if (name == "SpriteComponent") return std::make_unique<SpriteComponent>();

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
		else if (name == "Drawable") return typeID<sf3d::Drawable>();

		return 0;
	}

	///
	static std::string getNameFromIdentifier(size_t id)
	{
		if (id == typeID<SpriteComponent>()) return "SpriteComponent";
		else if (id == typeID<sf3d::Drawable>()) return "Drawable";

		return "";
	}

};

}
