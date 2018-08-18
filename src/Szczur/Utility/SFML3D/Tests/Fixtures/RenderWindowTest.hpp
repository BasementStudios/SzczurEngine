#pragma once

#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Utility/Tests.hpp"

struct RenderWindowTest : public ::testing::Test
{
	rat::Window* windowModule;
	sf3d::RenderWindow* renderWindow;
	
	virtual void SetUp() 
	{
		// Module
		this->windowModule = rat::detail::globalPtr<rat::Window>;
		this->windowModule->clear({24.f/255.f, 20.f/255.f, 28.f/255.f, 1.f});
		
		// Window
		this->renderWindow = &(this->windowModule->getWindow());
	}

	virtual void TearDown()
	{
		// Render
		this->windowModule->render();
	}
};
