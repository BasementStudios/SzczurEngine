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
	void _updateVisible() override;
	void _updateBlendMode() override;
	void _updateColor() override;

protected:
	void _initDisplay(void* value, bool isRetain) override;
	void _disposeDisplay(void* value, bool isRelease) override;
	void _onUpdateDisplay() override;
	void _addDisplay() override;
	void _replaceDisplay(void* value, bool isArmatureDisplay) override;
	void _removeDisplay() override;
	void _updateZOrder() override;

	void _updateFrame() override;
	void _updateMesh() override;
	void _updateTransform() override;
	void _identityTransform() override;

	void _onClear() override;
};

DRAGONBONES_NAMESPACE_END
