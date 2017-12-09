#pragma once

#include <memory>

#include <dragonBones/SFMLArmatureDisplay.h>

#include <SFML\Graphics\Drawable.hpp>

namespace rat
{
	class Armature : public sf::Drawable 
	{
	private:
		std::unique_ptr<dragonBones::SFMLArmatureDisplay> _armatureDisplay;

	public:
		Armature() = default;
		Armature(std::string_view name);
		~Armature() = default;

		void create(std::string_view name);

		void setPosition(const sf::Vector2f& pos) { _armatureDisplay->setPosition(pos); }
		auto& getPosition() { return _armatureDisplay->getPosition(); }

		void playAnimation(std::string_view animation) { _armatureDisplay->getAnimation()->play(animation.data()); }

	protected:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	};
}
