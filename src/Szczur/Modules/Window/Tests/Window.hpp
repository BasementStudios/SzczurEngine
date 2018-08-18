#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include "Szczur/Utility/Tests.hpp"

struct SimpleWindowTest : public ::testing::Test
{
	rat::Window* windowModule;
	
	virtual void SetUp() 
	{
		// Module
		this->windowModule = rat::detail::globalPtr<rat::Window>;
		this->windowModule->clear({24.f/255.f, 20.f/255.f, 28.f/255.f, 1.f});
	}

	virtual void TearDown()
	{
		// Render
		this->windowModule->render();
	}
};

VISUAL_TEST_F(SimpleWindowTest, Clear)
{
	windowModule->clear({0.f, 0.f, 0.5f, 1.f});
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
		windowModule->clear({24.f/255.f, 20.f/255.f, 28.f/255.f, 1.f});
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
