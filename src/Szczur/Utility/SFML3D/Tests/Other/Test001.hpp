#pragma once

#include "Szczur/Utility/SFML3D/ShaderProgram.hpp"
#include "Szczur/Utility/SFML3D/Shader.hpp"
#include "Szczur/Utility/SFML3D/RenderLayer.hpp"
#include "Szczur/Utility/SFML3D/Texture.hpp"
#include "Szczur/Utility/SFML3D/Sprite.hpp"
#include "../Fixtures/RenderTargetTest.hpp"
#include "Szczur/Utility/Tests.hpp"

struct Test001_RenderLayerTest : public RenderTargetTest
{
	sf3d::RenderLayer backgroundLayer;
	sf3d::RenderLayer foregroundLayer;
	
	sf3d::ShaderProgram layerShaderProgram;

	sf3d::ShaderProgram targetShaderProgram;
	
	virtual void SetUp() 
	{
		RenderTargetTest::SetUp();

		// Target
		{
			// Shader
			this->targetShaderProgram.linkShaders( // @todo to powinien być już weather shader
				sf3d::Shader {sf3d::Shader::Vertex, 	"Assets/Shaders/assemble.vert"},
				sf3d::Shader {sf3d::Shader::Fragment, 	"Assets/Shaders/assemble.frag"}
			);
			this->renderTarget->setDefaultShaderProgram(targetShaderProgram);
		}
		
		// Shader for layers
		this->layerShaderProgram.linkShaders(
			sf3d::Shader {sf3d::Shader::Vertex, 	"Assets/Shaders/view.vert"},
			sf3d::Shader {sf3d::Shader::Fragment, 	"Assets/Shaders/Tests/001/fragment.frag"}
		);

		this->layerShaderProgram.loadConfig("Assets/Shaders/Tests/001/config.json");

		// Background layer
		{
			// Create
			glEnable(GL_DEPTH_TEST);
			this->backgroundLayer.create(this->windowModule->getSize());
			this->backgroundLayer.clear({0.f, 0.f, 0.f, 0.f}, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			// Select shader
			this->backgroundLayer.setDefaultShaderProgram(layerShaderProgram);

			// Reset position factor
			this->backgroundLayer.positionFactor = 1.f;
		}

		// Foreground layer
		{
			// Create
			glEnable(GL_DEPTH_TEST);
			this->foregroundLayer.create(this->windowModule->getSize());
			this->foregroundLayer.clear({0.f, 0.f, 0.f, 0.f}, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			// Select shader
			this->foregroundLayer.setDefaultShaderProgram(layerShaderProgram);

			// Reset position factor
			this->foregroundLayer.positionFactor = 1.f;
		}
	}

	virtual void TearDown()
	{
		// Draw layer
		glDisable(GL_DEPTH_TEST);
		
		RenderTargetTest::TearDown();
	}
};

TEST_F(Test001_RenderLayerTest, Case1)
{
	// Background
	sf3d::Texture backgroundTexture("Assets/Shaders/Tests/001/background.png");
	sf3d::Sprite backgroundSprite(backgroundTexture);
	backgroundSprite.move({-1.f, 1.f, 0.f});

	// Foreground
	sf3d::Texture foregroundTexture("Assets/Shaders/Tests/001/foreground.png");
	sf3d::Sprite foregroundSprite(foregroundTexture);
	foregroundSprite.move({-1.f, 1.f, 0.f});
	
	VISUAL_TEST_LOOP({
		// Update
		layerShaderProgram.setUniform("time", clock.getElapsedTime().asFSeconds());
	}, {
		// Render
		glEnable(GL_DEPTH_TEST);

		// Background layer
		backgroundLayer.clear({0.f, 0.f, 0.f, 0.f}, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		backgroundLayer.draw(backgroundSprite);

		// Foreground layer
		foregroundLayer.clear({0.f, 0.f, 0.f, 0.f}, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		foregroundLayer.draw(foregroundSprite);

		// Window
		glDisable(GL_DEPTH_TEST);
		windowModule->clear({24.f/255.f, 20.f/255.f, 28.f/255.f, 1.f});
		renderTarget->draw(backgroundLayer);
		renderTarget->draw(foregroundLayer);
		windowModule->render();
	}, {
		// Input
		switch (event.type) {
			case sf::Event::Closed:
			case sf::Event::KeyReleased:
				testing = false;
				break;
			default:
				break;
		}
	});
}

// @todo , more tests
