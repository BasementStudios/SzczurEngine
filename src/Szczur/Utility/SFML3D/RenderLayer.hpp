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
#include "SimpleSprite.hpp"
#include "RenderStates.hpp"

namespace sf3d
{

class RenderLayer : public RenderTexture, public Drawable
{
	/* Variables */
protected:
	sf3d::SimpleSprite sprite; // @todo . use directly rendering



	/* Operators */
public:
	RenderLayer();
	~RenderLayer();

	RenderLayer(glm::uvec2 size, ShaderProgram* program = nullptr);



	/* Methods */
public:
	void create(glm::uvec2 size, ShaderProgram* program = nullptr);

	void draw(RenderTarget& target, RenderStates states = RenderStates::Default) const override;
};

}
