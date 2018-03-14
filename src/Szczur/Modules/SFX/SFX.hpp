#pragma once

#include "ShaderManager.hpp"
#include "Szczur/Utility/Modules/Module.hpp"

namespace rat
{

class SFX : public Module<>
{
public:

	SFX();

	~SFX();

	SFX(const SFX&) = delete;

	SFX& operator = (const SFX&) = delete;

	SFX(SFX&&) = delete;

	SFX& operator = (SFX&&) = delete;

	void update();

	ShaderManager& getManager();
	const ShaderManager& getManager() const;

private:

	ShaderManager _manager;

};

}
