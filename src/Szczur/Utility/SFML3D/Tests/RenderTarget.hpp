#pragma once

#include <chrono>
#include <thread>
#include <memory>

#include <SFML/Graphics.hpp>
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Utility/SFML3D/RenderLayer.hpp"
#include "Szczur/Utility/SFML3D/CircleShape.hpp"
#include "Szczur/Utility/SFML3D/RectangleShape.hpp"
#include "Szczur/Utility/Tests.hpp"

struct RenderTargetTest : public ::testing::Test
{
	rat::Window* windowModule;
	sf3d::RenderTarget* renderTarget;
	sf3d::ShaderProgram shaderProgram;
	
	virtual void SetUp() 
	{
		// Window
		this->windowModule = rat::detail::globalPtr_v<rat::Window>;
		this->windowModule->clear({24u, 20u, 28u, 255u});
		
		// Target
		this->renderTarget = &(this->windowModule->getWindow());

		// Shader
		this->shaderProgram.linkShaders(
			sf3d::Shader {sf3d::Shader::Vertex, 	"Assets/Shaders/simple/vertex.vert"},
			sf3d::Shader {sf3d::Shader::Fragment, 	"Assets/Shaders/simple/vertex.frag"}
		);
		this->windowModule->getWindow().setDefaultShaderProgram(shaderProgram);
	}

	virtual void TearDown()
	{
		// Render
		this->windowModule->render();

		// Wait
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(333ms);
	}
};

TEST_F(RenderTargetTest, DrawBasic)
{
	sf3d::RectangleShape object({0.3f, 0.2f});
	renderTarget->draw(object);
}
// TEST_F(RenderTargetTest, DrawMove)
// {
// 	sf3d::RectangleShape object({0.3f, 0.2f});
// 	object.move();	
// 	renderTarget.draw(object);
// }
