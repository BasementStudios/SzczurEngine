#pragma once

#include "Component.hpp"
#include "SpriteDisplayData.hpp"
#include <Szczur/Utility/SFML3D/Drawable.hpp>
#include <Szczur/Utility/SFML3D/RenderTarget.hpp>
#include <Szczur/Utility/SFML3D/RenderStates.hpp>

namespace rat
{

	class SpriteComponent : public sf3d::Drawable, public Component
	{
	public:	
		///
		SpriteComponent(SpriteDisplayData* spriteDisplayData = nullptr): 
		Component{ "SpriteComponent", typeID<SpriteComponent>(), typeID<sf3d::Drawable>() },
		_spriteDisplayData(spriteDisplayData)
		{

		}

		///
		SpriteComponent(const SpriteComponent&) = default;

		///
		SpriteComponent& operator = (const SpriteComponent&) = default;

		///
		SpriteComponent(SpriteComponent&&) = default;

		///
		SpriteComponent& operator = (SpriteComponent&&) = default;

		///
		~SpriteComponent() = default;

		///
		virtual std::unique_ptr<Component> copy() const override
		{
			return std::make_unique<SpriteComponent>(*this);
		}

		///
		void setSpriteDisplayData(SpriteDisplayData* spriteDisplayData)
		{
			_spriteDisplayData = spriteDisplayData;
		}

		///
		SpriteDisplayData* getSpriteDisplayData() const
		{
			return _spriteDisplayData;
		}

		///
		virtual void* getFeature(size_t featureID) override
		{
			if (featureID == typeID<sf3d::Drawable>())
				return static_cast<sf3d::Drawable*>(this);

			return nullptr;
		}

		///
		virtual const void* getFeature(size_t featureID) const override
		{
			if (featureID == typeID<sf3d::Drawable>())
			{
				return static_cast<const sf3d::Drawable*>(this);
			}

			return nullptr;
		}

		///
		virtual void loadFromConfig(const Json& config) override
		{
			//setTextureID(config["textureID"].get<int>());
			//setVerticesCount(config["verticesCount"].get<size_t>());
		}

		///
		virtual void saveToConfig(Json& config) const override
		{
			config["name"] = getName();
			//config["textureID"] = getTextureID();
			//config["verticesCount"] = getVerticesCount();
		}

		virtual void draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const override {
			if(_spriteDisplayData)
				target.draw(*_spriteDisplayData, states);
		}

	private:
		SpriteDisplayData* _spriteDisplayData;

	};

}
