/** @file SF3DNode.hpp
** @author Piotr Krupa (piotrkrupa06@gmail.com)
**/

#pragma once

#include <dragonBones/DragonBonesHeaders.h>

#include <SFML/Graphics/Rect.hpp>

#include "Szczur/Utility/SFML3D/Drawable.hpp"

DRAGONBONES_NAMESPACE_BEGIN

class SF3DNode : public sf3d::Drawable
{
protected:
	sf3d::Transform _transform;

	bool _visible = true;

	float _zOffset = 0.f;


public:
	SF3DNode() = default;
	virtual ~SF3DNode() = default;

	void setZOffset(float zOffset) { _zOffset = zOffset; }
	float getZOffset() const { return _zOffset; }

	void setMatrix(const Matrix& matrix, const glm::vec3& offset, float scaleX, float scaleY)
	{
		auto& mat = this->_transform.getMatrix();

		mat = glm::mat4(
			matrix.a, matrix.b, 0.f, 0.f,
			matrix.c, matrix.d, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			offset.x, offset.y, offset.z, 1.f
		);

		this->_transform.scale(scaleX, scaleY, 1.f);
	}

	virtual void setVisible(bool visible) { _visible = visible; }

	virtual void setColor(const dragonBones::ColorTransform& color) = 0;

	virtual sf::FloatRect getBoundingBox() = 0;
};

DRAGONBONES_NAMESPACE_END
