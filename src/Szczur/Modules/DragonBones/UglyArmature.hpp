#pragma once

#include <string>
#include <memory>

#include <dragonBones/animation/Animation.h>
#include <dragonBones/SFMLArmatureDisplay.h>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>

#include "Szczur/Modules/Script/Script.hpp"

namespace rat
{

class UglyArmature : public sf::Drawable 
{
	/* Fields */
private:
	std::unique_ptr<dragonBones::SFMLArmatureDisplay> display;

	sf::Vector2f scale {1.f, 1.f};



	/* Properties */
public:
	void setPosition(const sf::Vector2f& pos) 
		{ this->display->setPosition(pos); }
	const sf::Vector2f& getPosition() const 
		{ return this->display->getPosition(); }

	void setScale(const sf::Vector2f& scale) 
		{ this->scale = scale; }
	const sf::Vector2f& getScale() const 
		{ return this->scale; }

	void setTimeScale(float timeScale) 
		{ this->display->getAnimation()->timeScale = timeScale; }
	float getTimeScale() 
		{ return this->display->getAnimation()->timeScale; }

	void setFlipX(bool value) 
		{ this->display->getArmature()->setFlipX(value); }
	bool getFlipX() const 
		{ return this->display->getArmature()->getFlipX(); }

	void setFlipY(bool value) 
		{ this->display->getArmature()->setFlipY(value); }
	bool getFlipY() const 
		{ return this->display->getArmature()->getFlipY(); }

	dragonBones::Animation* getAnimation()
		{ return this->display->getAnimation(); }



	/* Operators */
public:
	UglyArmature(dragonBones::SFMLArmatureDisplay* armatureDisplay);
	~UglyArmature() = default;



	/* Methods */
public:
	void playAnimation(const std::string& animation = "", int playTimes = -1) 
		{ this->display->getAnimation()->play(animation.data(), playTimes); }
	void stopAnimation(const std::string& animation) 
		{ this->display->getAnimation()->stop(animation.data()); }
	void fadeInAnimation(const std::string& animationName, float fadeInTime = -1.f, int playTimes = -1, int layer = 0, 
							const std::string& group = "", dragonBones::AnimationFadeOutMode fadeOutMode = dragonBones::AnimationFadeOutMode::SameLayerAndGroup) 
		{ this->display->getAnimation()->fadeIn(animationName.data(), fadeInTime, playTimes, layer, group, fadeOutMode); }

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;



	/* Functions */
public:
	static void initScript(Script& script) {
		auto object = script.newClass<UglyArmature>("UglyArmature", "DragonBones");
		
		object.set("setPosition", &UglyArmature::setPosition);
		object.set("getPosition", &UglyArmature::getPosition);
		object.set("setScale", &UglyArmature::setScale);
		object.set("getScale", &UglyArmature::getScale);
		object.set("setTimeScale", &UglyArmature::setTimeScale);
		object.set("getTimeScale", &UglyArmature::getTimeScale);
		object.set("setFlipX", &UglyArmature::setFlipX);
		object.set("getFlipX", &UglyArmature::getFlipX);
		object.set("setFlipY", &UglyArmature::setFlipY);
		object.set("getFlipY", &UglyArmature::getFlipY);
		
		object.set("playAnimation", &UglyArmature::playAnimation);
		object.set("stopAnimation", &UglyArmature::stopAnimation);
		object.set("fadeInAnimation", &UglyArmature::fadeInAnimation);
		object.set("setFlipY", &UglyArmature::setFlipY);
		
		object.init();
	}
};

}
