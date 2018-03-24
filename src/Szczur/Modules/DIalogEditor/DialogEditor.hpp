#pragma once

#include "Szczur/Utility/Modules/Module.hpp"

namespace rat
{

class DialogEditor : public Module<>
{
public:
	DialogEditor();
	~DialogEditor();

	void update();
};

}
