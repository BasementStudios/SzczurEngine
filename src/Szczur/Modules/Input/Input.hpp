#pragma once

#include "InputManager.hpp"
#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/Script/Script.hpp"

namespace rat
{

class Input : public Module<Script>
{
public:

	///
	Input();

	///
	Input(const Input&) = delete;

	///
	Input& operator = (const Input&) = delete;

	///
	Input(Input&&) = delete;

	///
	Input& operator = (Input&&) = delete;

	///
	~Input();

	///
	InputManager& getManager();

	///
	const InputManager& getManager() const;

private:

	InputManager _inputManager;

};

}
