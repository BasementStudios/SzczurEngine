/** @file SF3DDisplay.hpp
** @author Piotr Krupa (piotrkrupa06@gmail.com)
** @author Patryk (PsychoX) Ludwikowski <psychoxivi@gmail.com>
** @author Kuba (Metriko)
**/

#pragma once

#include <vector>

#include <dragonBones/DragonBonesHeaders.h>
#include <glad/glad.h> // vec3
#include <SFML/Graphics/Color.hpp>

#include "Szczur/Utility/SFML3D/Texture.hpp"
#include "Szczur/Utility/SFML3D/VertexArray.hpp"
#include "Szczur/Utility/SFML3D/RenderTarget.hpp"
#include "Szczur/Utility/SFML3D/RenderStates.hpp"
#include "Szczur/Utility/SFML3D/Sprite.hpp"

DRAGONBONES_NAMESPACE_BEGIN

/** @class SF3DDisplay
**/
class SF3DDisplay
{
	/* Fields */
public:
	const sf3d::Texture* texture = nullptr;

	std::vector<std::vector<int>> 			verticesInTriagles;
	sf3d::VertexArray		 				verticesDisplay{ 4 };

	bool visible = true;

	// sf::BlendMode blendMode; // @todo . BlendMode

	GLuint primitiveType = GL_TRIANGLE_STRIP;

protected:
	sf3d::Transform transform;



	/* Operators */
public:
	SF3DDisplay() = default;
	~SF3DDisplay() = default;

	/* Methods */
public:
	void setMatrix(const Matrix& matrix, const glm::vec3& offset = { 0.f, 0.f, 0.f }, const float& scale = 1.f)
	{
		auto& mat = this->transform.getMatrix();

		mat = glm::mat4(
			matrix.a, matrix.b, 0.f, 0.f,
			matrix.c, matrix.d, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			offset.x, offset.y, offset.z, 1.f
		);

		this->transform.scale(scale, scale, 1.f);
	}

	void setColor(const dragonBones::ColorTransform& color)
	{
		for (std::size_t i = 0; i < this->verticesDisplay.getSize(); ++i)
		{
			this->verticesDisplay.setColor(i, { color.redMultiplier, color.greenMultiplier, color.blueMultiplier, color.alphaMultiplier });
		}
	}

	void draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
	{
		if (visible)
		{
			// states.blendMode = blendMode; // @todo . BlendMode
			states.texture = texture;
			states.transform *= this->transform;
			target.draw(this->verticesDisplay, states);
		}
	}
};

DRAGONBONES_NAMESPACE_END
