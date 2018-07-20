#pragma once

#include "InputManager.hpp"
#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Script/Script.hpp"

namespace rat
{

class Input : public Module<Window, Script>
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
