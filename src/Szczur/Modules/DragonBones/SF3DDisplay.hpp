/** @file SF3DDisplay.hpp
** @author Piotr Krupa (piotrkrupa06@gmail.com)
** @author Patryk (PsychoX) Ludwikowski <psychoxivi@gmail.com>
** @author Kuba (Metriko)
**/

#pragma once

#include <vector>

#include <SFML/Graphics/Rect.hpp>

#include <dragonBones/DragonBonesHeaders.h>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>

#include "Szczur/Utility/SFML3D/Texture.hpp"
#include "Szczur/Utility/SFML3D/VertexArray.hpp"
#include "Szczur/Utility/SFML3D/RenderTarget.hpp"
#include "Szczur/Utility/SFML3D/RenderStates.hpp"

#include "SF3DNode.hpp"

DRAGONBONES_NAMESPACE_BEGIN

/** @class SF3DDisplay
 **/
class SF3DDisplay : public SF3DNode
{
	/* Fields */
public:
	const sf3d::Texture* texture = nullptr;

	std::vector<std::vector<int>>	verticesInTriagles;
	sf3d::VertexArray		 		verticesDisplay {4u, sf3d::PrimitiveType::TriangleFan};

	// sf::BlendMode blendMode; // @todo . BlendMode

	GLuint primitiveType = GL_TRIANGLE_STRIP;

protected:

	/* Operators */
public:
	SF3DDisplay() = default;
	~SF3DDisplay() = default;

	/* Methods */
public:
	void setColor(const dragonBones::ColorTransform& color) override
	{
		for (std::size_t i = 0; i < this->verticesDisplay.getSize(); ++i)
		{
			this->verticesDisplay[i].color = { color.redMultiplier, color.greenMultiplier, color.blueMultiplier, color.alphaMultiplier };
		}
	}

	void draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
	{
		if (_visible)
		{
			// states.blendMode = blendMode; // @todo . BlendMode
			states.texture = texture;
			states.transform *= this->_transform;
			target.draw(this->verticesDisplay, states);
		}
	}

	sf::FloatRect getBoundingBox() override
	{
		if (texture == nullptr)
			return sf::FloatRect();

		if (verticesDisplay.getSize() == 0)
			return sf::FloatRect();

		glm::vec3 min = verticesDisplay[0].position;
		glm::vec3 max = min;

		for (std::size_t i = 0; i < verticesDisplay.getSize(); i++)
		{
			auto& vert = verticesDisplay[i];

			min.x = std::min(min.x, vert.position.x);
			min.y = std::min(min.y, vert.position.y);
			max.x = std::max(max.x, vert.position.x);
			max.y = std::max(max.y, vert.position.y);
		}

		sf::FloatRect rect(min.x, min.y, (max - min).x, (max - min).y);

		sf::Transform bbMatrix;

		auto mat = _transform.getMatrix();

		bbMatrix.combine(sf::Transform(
			mat[0][0], mat[1][0], mat[3][0],
			mat[0][1], mat[1][1], mat[3][1],
			0.f, 0.f, 1.f
		));

		rect = bbMatrix.transformRect(rect);

		return rect;
	}
};

DRAGONBONES_NAMESPACE_END
