#include "Armature.hpp"

#include <dragonBones/SFMLFactory.h>
#include <dragonBones/SFMLArmatureDisplay.h>

namespace rat
{
	Armature::Armature(dragonBones::SFMLArmatureDisplay* armatureDisplay)
		: _armatureDisplay(armatureDisplay)
	{
		
	}

	void Armature::replaceSlotsTexture(const std::string& slotName, sf::Texture* texture)
	{
		if (!texture)
			return;

		if (!_armatureDisplay)
			return;

		auto slot = _armatureDisplay->getArmature()->getSlot(slotName.data());

		if (!slot)
			return;
	
		auto display = static_cast<dragonBones::SFMLDisplay*>(slot->getRawDisplay());

		if (display) 
			display->texture = texture;
		
	}

	void Armature::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (_armatureDisplay) {
			states.transform.scale({ _scale, _scale }, getPosition());
			target.draw(*_armatureDisplay, states);
		}
	}
}
