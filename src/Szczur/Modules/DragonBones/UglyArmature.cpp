#include "UglyArmature.hpp"

#include <dragonBones/SFMLFactory.h>
#include <dragonBones/SFMLArmatureDisplay.h>

namespace rat
{
	UglyArmature::UglyArmature(dragonBones::SFMLArmatureDisplay* display)
		: display(display)
	{
	}

	void UglyArmature::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (this->display) {
			states.transform.scale(this->scale, getPosition());
			target.draw(*(this->display), states);
		}
	}
}
