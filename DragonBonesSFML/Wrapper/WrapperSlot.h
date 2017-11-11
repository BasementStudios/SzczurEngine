#pragma once

#include <memory>

#include <dragonBones\DragonBonesHeaders.h>

#include <SFML\Graphics.hpp>

#include "WrapperDisplay.h"

class WrapperSlot : public dragonBones::Slot
{
	BIND_CLASS_TYPE_A(WrapperSlot);

private:
	float _textureScale;
	std::unique_ptr<WrapperDisplay> _renderDisplay;

public:
	virtual void _updateVisible() override;
	virtual void _updateBlendMode() override;
	virtual void _updateColor() override;

protected:
	virtual void _initDisplay(void * value) override;
	virtual void _disposeDisplay(void * value) override;
	virtual void _onUpdateDisplay() override;
	virtual void _addDisplay() override;
	virtual void _replaceDisplay(void * value, bool isArmatureDisplay) override;
	virtual void _removeDisplay() override;
	virtual void _updateZOrder() override;

	virtual void _updateFrame() override;
	virtual void _updateMesh() override;
	virtual void _updateTransform(bool isSkinnedMesh) override;

	virtual void _onClear() override;
};

