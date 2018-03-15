#pragma once

#include <SFML/Graphics.hpp>

#include "ShaderManager.hpp"
#include "Szczur/Utility/Modules/Module.hpp"

namespace rat
{

class SPFX : public Module<>
{
public:

	SPFX();

	~SPFX();

	SPFX(const SPFX&) = delete;

	SPFX& operator = (const SPFX&) = delete;

	SPFX(SPFX&&) = delete;

	SPFX& operator = (SPFX&&) = delete;

	void update();

	ShaderManager& getManager();
	const ShaderManager& getManager() const;

private:

	ShaderManager _manager;

	#ifdef EDITOR
	bool _isEditorOpen;
	sf::RenderTexture _previewRTex;
	#endif

};

}
