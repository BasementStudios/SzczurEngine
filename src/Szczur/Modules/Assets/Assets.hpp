#pragma once

#include "Szczur/Utility/Modules.hpp"

#include "AssetsManager.hpp"

namespace rat
{

class Assets : public ModuleBase<>
{
	using ModuleBase::ModuleBase;

public:

	void init();

	void input(const sf::Event& event);

	void update(float deltaTime);

	void render();

	void loadFromJsonFile(const std::string& path);

};

}
