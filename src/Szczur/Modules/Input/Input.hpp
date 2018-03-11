#pragma once

#include "InputManager.hpp"
#include "Szczur/Utility/Modules/Module.hpp"

namespace rat
{

class Input : public Module<>
{
public:

	Input();

	Input(const Input&) = delete;

	Input& operator = (const Input&) = delete;

	Input(Input&&) = delete;

	Input& operator = (Input&&) = delete;

	~Input();

	InputManager& getManager();
	const InputManager& getManager() const;

private:

	InputManager _inputManager;

};

}
