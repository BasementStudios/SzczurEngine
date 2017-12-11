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

		float _scale = 1.f;

	public:
		Armature() = default;
		Armature(const std::string& name);
		~Armature() = default;

		void create(const std::string& name);

		void setPosition(const sf::Vector2f& pos) { _armatureDisplay->setPosition(pos); }
		auto& getPosition() const { return _armatureDisplay->getPosition(); }

		void setScale(float scale) { _scale = scale; }
		float getScale() const { return _scale; }

		void setFlipX(bool value) { _armatureDisplay->getArmature()->setFlipX(value); }
		bool getFlipX() const { return _armatureDisplay->getArmature()->getFlipX(); }

		void setFlipY(bool value) { _armatureDisplay->getArmature()->setFlipY(value); }
		bool getFlipY() const { return _armatureDisplay->getArmature()->getFlipY(); }

		void playAnimation(const std::string& animation) { _armatureDisplay->getAnimation()->play(animation.data()); }
		void stopAnimation(const std::string& animation) { _armatureDisplay->getAnimation()->stop(animation.data()); }
		void fadeInAnimation(const std::string& animationName, float fadeInTime = -1.f, int playTimes = -1, int layer = 0, 
							 const std::string& group = "", dragonBones::AnimationFadeOutMode fadeOutMode = dragonBones::AnimationFadeOutMode::SameLayerAndGroup) 
		{ 
			_armatureDisplay->getAnimation()->fadeIn(animationName.data(), fadeInTime, playTimes, layer, group, fadeOutMode); 
		}

		void addEvent(const std::string& type, const std::function<void(dragonBones::EventObject*)>& listener) { _armatureDisplay->addDBEventListener(type, listener); }

		void replaceSlotsTexture(const std::string& slotName, sf::Texture* texture);

	protected:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	};
}
