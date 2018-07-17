#pragma once

/** @file RenderLayer.hpp
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <glm/vec2.hpp>

namespace sf3d {
	class ShaderProgram;
	class RenderTarget;
}
#include "Drawable.hpp"
#include "RenderTexture.hpp"
#include "VertexArray.hpp"
#include "RenderStates.hpp"

namespace sf3d
{

class RenderLayer : public RenderTexture, public Drawable
{
	/* Variables */
protected:
	sf3d::VertexArray vertices {4u};



	/* Operators */
public:
	RenderLayer();
	~RenderLayer();

	RenderLayer(glm::uvec2 size, ShaderProgram* program = nullptr);



	/* Methods */
public:
	void create(glm::uvec2 size, ShaderProgram* program = nullptr);

	void draw(RenderTarget& target, RenderStates states = RenderStates::Default) const override;
	using sf3d::RenderTarget::draw;
};

}
