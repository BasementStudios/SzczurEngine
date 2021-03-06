#pragma once

#include "Szczur/Utility/SFML3D/ShaderProgram.hpp"
#include "Szczur/Utility/SFML3D/Shader.hpp"
#include "Szczur/Utility/SFML3D/RectangleShape.hpp"
#include "./Fixtures/RenderTargetTest.hpp"
#include "Szczur/Utility/Tests.hpp"

struct SimpleRenderTargetTest : public RenderTargetTest
{
	sf3d::ShaderProgram shaderProgram;
	
	virtual void SetUp() 
	{
		RenderTargetTest::SetUp();

		// Shader
		this->shaderProgram.linkShaders(
			sf3d::Shader {sf3d::Shader::Vertex, 	"Assets/Shaders/model.vert"},
			sf3d::Shader {sf3d::Shader::Fragment, 	"Assets/Shaders/color.frag"}
		);
		this->renderWindow->setDefaultShaderProgram(shaderProgram);

		// Reset position factor
		this->renderTarget->positionFactor = 1.f;
	}
};

VISUAL_TEST_F(SimpleRenderTargetTest, Clear)
{
	renderTarget->clear({0u, 0u, 128u, 255u}, GL_COLOR_BUFFER_BIT);
}

VISUAL_TEST_F(SimpleRenderTargetTest, DrawBasic)
{
	sf3d::RectangleShape object({0.3f, 0.2f});
	renderTarget->draw(object);
}

VISUAL_TEST_F(SimpleRenderTargetTest, DrawMove)
{
	sf3d::RectangleShape object({0.3f, 0.2f});
	object.move({-0.5f, 0.1f, 0.f});
	renderTarget->draw(object);
}

VISUAL_TEST_F(SimpleRenderTargetTest, DrawRotate)
{
	sf3d::RectangleShape object({0.3f, 0.2f});
	object.rotate({30.f, 40.f, 10.f});
	renderTarget->draw(object);
}

// @todo , more tests
