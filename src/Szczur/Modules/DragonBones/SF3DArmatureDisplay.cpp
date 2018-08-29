#include "SF3DArmatureDisplay.hpp"

#include "SF3DArmatureProxy.hpp"

DRAGONBONES_NAMESPACE_BEGIN

SF3DArmatureDisplay::SF3DArmatureDisplay(const std::string& armatureName, const std::string& dragonBonesName, const std::string& skinName, const std::string& textureAtlasName)
{
	_proxy = SF3DFactory::get()->buildArmatureDisplay(armatureName, dragonBonesName, skinName, textureAtlasName);
}

SF3DArmatureDisplay::~SF3DArmatureDisplay()
{
	_proxy->dispose(true);
	_proxy = nullptr;
}

void SF3DArmatureDisplay::update(float deltaTime)
{
	if (_proxy)
		_proxy->advanceTime(deltaTime);
}

void SF3DArmatureDisplay::draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
{
	if (_proxy)
		_proxy->draw(target, states);
}

DRAGONBONES_NAMESPACE_END
