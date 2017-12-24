#pragma once

#include "Szczur/Utility/Modules.hpp"

#include "AssetsManager.hpp"

namespace rat
{

class Assets : public Module<>
{
	using Module::Module;

public:

	void init();

	void loadFromJsonFile(const std::string& path);

};

}
