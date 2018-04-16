#pragma once

/** @file SF3DSlot.hpp
 ** @author Piotr Krupa (piotrkrupa06@gmail.com)
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi@gmail.com>
 **/

#include <memory> // unique_ptr

#include <dragonBones/DragonBonesHeaders.h>

#include "SF3DDisplay.hpp"

DRAGONBONES_NAMESPACE_BEGIN

class SF3DSlot : public Slot
{
	BIND_CLASS_TYPE_A(SF3DSlot);

	/* Fields */
private:
	float _textureScale;
	std::unique_ptr<SF3DDisplay> _renderDisplay;

	/** Methods */
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
