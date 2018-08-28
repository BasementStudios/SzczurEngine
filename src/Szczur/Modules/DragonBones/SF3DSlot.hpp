/** @file SF3DSlot.hpp
** @author Piotr Krupa (piotrkrupa06@gmail.com)
**/

#pragma once

#include <memory>

#include <dragonBones/DragonBonesHeaders.h>

DRAGONBONES_NAMESPACE_BEGIN

class SF3DNode;

class SF3DSlot : public Slot
{
	BIND_CLASS_TYPE_A(SF3DSlot);

private:
	float _textureScale;
	SF3DNode* _renderDisplay = nullptr;

	const float _zOffsetScale = 0.1f;

public:
	virtual void _updateVisible() override;
	virtual void _updateBlendMode() override;
	virtual void _updateColor() override;

protected:
	virtual void _initDisplay(void* value, bool isRetain) override;
	virtual void _disposeDisplay(void* value, bool isRelease) override;
	virtual void _onUpdateDisplay() override;
	virtual void _addDisplay() override;
	virtual void _replaceDisplay(void* value, bool isArmatureDisplay) override;
	virtual void _removeDisplay() override;
	virtual void _updateZOrder() override;

	virtual void _updateFrame() override;
	virtual void _updateMesh() override;
	virtual void _updateTransform() override;
	virtual void _identityTransform() override;

	virtual void _onClear() override;
};

DRAGONBONES_NAMESPACE_END
