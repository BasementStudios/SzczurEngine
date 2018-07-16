#pragma once

#include <chrono>
#include <thread>

#include "Szczur/Utility/Tests.hpp"

#include "Szczur/Modules/Window/Window.hpp"
#include "../RenderLayer.hpp"
#include "../CircleShape.hpp"
#include "../RectangleShape.hpp"
#include <SFML/Graphics.hpp>

TEST(sf3d_RenderLayer, WorldProjection)
{
	using namespace rat;
	using namespace sf3d;

	LOG_INFO("Getting window module");
	Window& windowModule = *(rat::detail::globalPtr_v<Window>);
	windowModule.clear({24u, 20u, 28u, 255u});

	LOG_INFO("Loading simple shader for Window");
	ShaderProgram simpleShaderProgram(
		Shader(Shader::Vertex, "Assets/Shaders/simple.vert"),
		Shader(Shader::Fragment, "Assets/Shaders/simple.frag")
	);
	windowModule.getWindow().setDefaultShaderProgram(simpleShaderProgram);

	// LOG_INFO("Rendering something on window (SFML)");
	// {
	// 	windowModule.pushGLStates();
	// 	sf::RectangleShape shapeOnWindow({300.f, 200.f});
	// 	shapeOnWindow.setPosition({10.f, 100.f});
	// 	shapeOnWindow.setOutlineColor(sf::Color::Red);
	// 	shapeOnWindow.setOutlineThickness(5);
	// 	windowModule.getWindow().draw(shapeOnWindow);
	// 	windowModule.popGLStates();
	// }

	LOG_INFO("Rendering something on window (SFML3D)");
	{
		//RectangleShape shapeOnWindow({1.8f, 1.8f});
		RectangleShape shapeOnWindow({0.9f, 0.9f});
		shapeOnWindow.move({-0.9.f, -0.9f, 0.f});
		windowModule.getWindow().draw(shapeOnWindow, RenderStates{Transform{}});
	}

	// LOG_INFO("Loading shader");
	// ShaderProgram shaderProgram(
	// 	Shader(Shader::Vertex, "Assets/Shaders/projection.vert"),
	// 	Shader(Shader::Fragment, "Assets/Shaders/world.frag")
	// );

	// LOG_INFO("Creating layer");
	// RenderLayer layer(windowModule.getSize(), &shaderProgram);

	//glEnable(GL_DEPTH_TEST);

	// LOG_INFO("Clearing layer with color");
	// layer.clear(0.f, 0.f, 1.f, 1.f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// LOG_INFO("Rendering something over layer");
	// {
	// 	CircleShape circleShape1;
	// 	layer.draw(circleShape1, {Transform{}});
	// }
	// CircleShape circleShape2;
	
	//glDisable(GL_DEPTH_TEST);

	// LOG_INFO("Drawing layer to the window");
	// windowModule.draw(layer);

	LOG_INFO("Display window");
	windowModule.getWindow().display();

	using namespace std::chrono_literals;
	std::this_thread::sleep_for(5s);
	return false;
}
