#pragma once

#include "Szczur/Utility/SFML3D/ShaderProgram.hpp"
#include "Szczur/Utility/SFML3D/Shader.hpp"
#include "Szczur/Utility/SFML3D/RenderLayer.hpp"
#include "./Fixtures/RenderTargetTest.hpp"
#include "Szczur/Utility/Tests.hpp"

struct SimpleRenderLayerTest : public RenderTargetTest
{
	sf3d::RenderLayer renderLayer;
	
	sf3d::ShaderProgram targetShaderProgram;
	sf3d::ShaderProgram layerShaderProgram;
	
	virtual void SetUp() 
	{
		RenderTargetTest::SetUp();

		// Shader for target
		this->targetShaderProgram.linkShaders(
			sf3d::Shader {sf3d::Shader::Vertex, 	"Assets/Shaders/assemble.vert"},
			sf3d::Shader {sf3d::Shader::Fragment, 	"Assets/Shaders/assemble.frag"}
		);
		this->renderTarget->setDefaultShaderProgram(targetShaderProgram);

		// Shader for layer
		this->layerShaderProgram.linkShaders(
			sf3d::Shader {sf3d::Shader::Vertex, 	"Assets/Shaders/model.vert"},
			sf3d::Shader {sf3d::Shader::Fragment, 	"Assets/Shaders/texture.frag"}
		);
		this->renderLayer.setDefaultShaderProgram(layerShaderProgram);
	}

	virtual void TearDown()
	{
		RenderTargetTest::TearDown();

		// Draw layer
		this->renderTarget->draw(this->renderLayer);
	}
};

TEST_F(SimpleRenderLayerTest, DrawBasic)
{
	sf3d::RectangleShape object({0.3f, 0.2f});
	renderLayer.draw(object);
	VISUAL_TESTING();
}
