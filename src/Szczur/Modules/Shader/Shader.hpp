#pragma once

#include <SFML/Graphics.hpp>

#include "ShaderManager.hpp"
#include "Szczur/Utility/Modules/Module.hpp"

namespace rat
{

class Shader : public Module<>
{
public:

	Shader();

	Shader(const Shader&) = delete;

	Shader& operator = (const Shader&) = delete;

	Shader(Shader&&) = delete;

	Shader& operator = (Shader&&) = delete;

	~Shader();

	void update();

	ShaderManager& getManager();
	const ShaderManager& getManager() const;

	#ifdef EDITOR
	bool isEditorOpen;
	sf::RenderTexture previewRTex;
	#endif

private:

	ShaderManager _manager;

};

}
