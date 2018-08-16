#pragma once

#include "Szczur/Utility/SFML3D/RenderTarget.hpp"
#include "RenderWindowTest.hpp"
#include "Szczur/Utility/Tests.hpp"

struct RenderTargetTest : public RenderWindowTest
{
	sf3d::RenderTarget* renderTarget;
	
	virtual void SetUp() 
	{
		RenderWindowTest::SetUp();
		
		// Target
		this->renderTarget = this->renderWindow;
	}
};
