#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include "Szczur/Utility/Tests.hpp"

struct SimpleWindowTest : public ::testing::Test
{
	rat::Window* windowModule;
	
	virtual void SetUp() 
	{
		// Module
		this->windowModule = rat::detail::globalPtr_v<rat::Window>;
		this->windowModule->clear({24u, 20u, 28u, 255u});
	}

	virtual void TearDown()
	{
		// Render
		this->windowModule->render();
	}
};

VISUAL_TEST_F(SimpleWindowTest, Clear)
{
	windowModule->clear({0u, 0u, 128u, 255u});
}

VISUAL_TEST_F(SimpleWindowTest, DrawSFML)
{
	windowModule->pushGLStates();
	sf::RectangleShape object({300.f, 200.f});
	windowModule->draw(object);
	windowModule->popGLStates();
}

TEST_F(SimpleWindowTest, Resize)
{
	sf::RectangleShape object({300.f, 200.f});
	object.move({10.f, 10.f});
	
	VISUAL_TEST_LOOP({
		// Update
	},{
		// Render
		windowModule->clear({24u, 20u, 28u});
		windowModule->pushGLStates();
		windowModule->draw(object);
		windowModule->popGLStates();
		windowModule->render();
	},{
		switch (event.type) {
			case sf::Event::Closed:
			case sf::Event::KeyReleased:
				testing = false;
				break;
			default:
				windowModule->processEvent(event);
				break;
		}	
	})
}

// @todo , more tests
