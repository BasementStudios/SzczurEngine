#pragma once

#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Utility/Tests.hpp"

struct RenderWindowTest : public ::testing::Test
{
	rat::Window* windowModule;
	sf3d::RenderWindow* renderWindow;
	
	virtual void SetUp() 
	{
		// Window
		this->windowModule = rat::detail::globalPtr_v<rat::Window>;
		this->windowModule->clear({24u, 20u, 28u, 255u});
		
		// Target
		this->renderWindow = &(this->windowModule->getWindow());
	}

	virtual void TearDown()
	{
		// Render
		this->windowModule->render();
	}
};
